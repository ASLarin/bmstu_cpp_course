<<<<<<< HEAD
cmake_minimum_required(VERSION 3.28.6)
=======
cmake_minimum_required(VERSION 3.28)
>>>>>>> c1bd6c8 (Обновлено# Please enter the commit message for your changes. Lines starting)
project(bmstu_cpp_course)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.12.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_subdirectory(tasks)
