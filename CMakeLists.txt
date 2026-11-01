cmake_minimum_required(VERSION 3.14)
project(MyProject)

# std::filesystem を使うため C++17 が必須
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Google Test の組み込み (FetchContent) ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# Windows環境などでVisual Studioのランタイム設定を合わせるためのおまじない
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# テスト機能を有効化
enable_testing()

# --- テスト用実行ファイルの定義 ---
# ※ターゲット名を RUN_TESTS（CMake自動生成）と衝突しないよう変更
add_executable(
  ReadingImpressionMakerTests
  tests/test_main.cpp
  tests/TestCsvLoader.cpp
  tests/TestQuestion.cpp
  tests/TestConsoleUI.cpp
  tests/TestRecordExporter.cpp
  src/CsvLoader.cpp
  src/ConsoleUI.cpp
  src/RecordExporter.cpp
)

# インクルードパスを通す
target_include_directories(ReadingImpressionMakerTests PRIVATE include)

# Google Testライブラリをリンク
target_link_libraries(
  ReadingImpressionMakerTests
  GTest::gtest
)

# テストを登録
add_test(NAME AllTests COMMAND ReadingImpressionMakerTests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})