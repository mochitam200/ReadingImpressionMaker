#include <iostream> // コンソールへテスト結果を表示するために使います。

int runQuestionTests(); // QuestionSetとReadingRecordのテストを実行する関数を宣言します。
int runCsvLoaderTests(); // CsvLoaderのテストを実行する関数を宣言します。
int runConsoleUITests(); // ConsoleUIのテストを実行する関数を宣言します。
int runRecordExporterTests(); // RecordExporterのテストを実行する関数を宣言します。

int main() // テストプログラムの開始地点です。
{
    std::cout << "========================================\n"; // テスト開始の枠を表示します。
    std::cout << " ReadingRecordMaker Unit Test\n"; // テストプログラムの名前を表示します。
    std::cout << " GoogleTest is NOT used\n"; // GoogleTestを使っていないことを表示します。
    std::cout << "========================================\n\n"; // テスト開始の枠を閉じます。

    int failedCount = 0; // 失敗したテストの数を保存します。

    failedCount += runQuestionTests(); // QuestionSetとReadingRecordのテストを実行します。
    failedCount += runCsvLoaderTests(); // CsvLoaderのテストを実行します。
    failedCount += runConsoleUITests(); // ConsoleUIのテストを実行します。
    failedCount += runRecordExporterTests(); // RecordExporterのテストを実行します。

    std::cout << "\n========================================\n"; // テスト終了の枠を表示します。
    std::cout << " Failed tests: " << failedCount << '\n'; // 失敗したテスト数を表示します。
    std::cout << "========================================\n"; // テスト終了の枠を閉じます。

    return failedCount == 0 ? 0 : 1; // 失敗がなければ0、1件以上あれば1を返します。
}
