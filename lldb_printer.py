#!/usr/bin/env python3

import lldb
from typing import List, Optional

class BMSTUListSyntheticProvider:
    def __init__(self, valobj: lldb.SBValue, internal_dict: dict):
        self.valobj = valobj
        self._nodes: List[lldb.SBValue] = []
        self._size = 0
        self.update()

    def update(self):
        """Обновляет внутреннее состояние"""
        self._nodes = []
        self._size = 0

        try:
            size_val = self.valobj.GetChildMemberWithName("size_")
            if not size_val.IsValid():
                return

            self._size = size_val.GetValueAsUnsigned(0)

            if self._size == 0:
                return

            head = self.valobj.GetChildMemberWithName("head_")
            tail = self.valobj.GetChildMemberWithName("tail_")

            if not head.IsValid() or not tail.IsValid():
                return

            current = head.GetChildMemberWithName("next_node_")
            tail_addr = tail.GetValueAsUnsigned()
            count = 0

            while current.IsValid() and count < self._size:
                current_addr = current.GetValueAsUnsigned()
                if current_addr == tail_addr:
                    break

                value = current.GetChildMemberWithName("value")
                if value.IsValid():
                    self._nodes.append(value)
                    count += 1

                current = current.GetChildMemberWithName("next_node_")

        except Exception as e:
            if self.valobj.GetTarget().GetDebugger().GetAsync():
                print(f"Error: {str(e)}")

    def num_children(self) -> int:
        return len(self._nodes) + 1  # +1 для элемента с размером

    def get_child_at_index(self, index: int) -> Optional[lldb.SBValue]:
        if index == 0:
            # Элемент с размером (будет отображаться как отдельная строка)
            return self.valobj.CreateValueFromExpression(
                "size",
                f"(size_t){self._size}"
            )
        elif 0 < index <= len(self._nodes):
            node = self._nodes[index-1]
            type_name = node.GetType().GetName()

            # Обработка std::string
            if "std::string" in type_name:
                summary = node.GetSummary() or ""
                clean_summary = summary.strip('"')
                return node.CreateValueFromData(
                    f'[{index-1}]',
                    node.GetData(),
                    node.GetType()
                )
            # Обработка int с подразделами
            else:
                value = node.GetValueAsSigned()
                return node.CreateValueFromExpression(
                    f'[{index-1}]',
                    f'(int){value}'
                )
        return None

def list_summary(valobj: lldb.SBValue, internal_dict: dict) -> str:
    """Выводит пустую строку, чтобы размер отображался отдельно"""
    return ""

def __lldb_init_module(debugger: lldb.SBDebugger, internal_dict: dict):
    """Регистрация с правильными именами"""
    module_name = "lldb_printer"

    debugger.HandleCommand(
        f'type synthetic add -x "^bmstu::list<.*>$" '
        f'--python-class {module_name}.BMSTUListSyntheticProvider'
    )

    debugger.HandleCommand(
        f'type summary add -x "^bmstu::list<.*>$" '
        f'--summary-string ""'  # Пустой summary
    )

    print(f"BMSTU list printer initialized (module: {module_name})")

# Использование:
# 1. Сохранить как 'lldb_printer.py'
# 2. В LLDB: (lldb) command script import lldb_printer.py