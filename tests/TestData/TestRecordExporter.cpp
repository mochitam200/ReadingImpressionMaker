#include "../../include/RecordExporter.hpp" // テスト対象のRecordExporterを使うために読み込みます。
#include <filesystem> // outputフォルダとファイルの存在を確認するために使います。
#include <fstream> // 出力ファイルを読み込むために使います。
#include <iostream> // テスト結果を表示するために使います。
#include <iterator> // ファイル内容を文字列へ読み込むために使います。
#include <string> // 文字列を扱うために使います。
#include <vector> // QuestionSetの配列を作るために使います。

namespace fs = std::filesystem; // std::filesystemを短くfsと書けるようにします。

namespace // このファイルだけで使う補助関数を名前空間で囲みます。
{ // 無名名前空間の開始です。

bool check(bool condition, const std::string& name) // テスト結果を共通表示する関数です。
{ // check関数の開始です。
    if (condition) // 期待した結果になったか確認します。
    { // 成功時の開始です。
        std::cout << "[PASS] " << name << '\n'; // 成功したテスト名を表示します。
        return true; // 成功を返します。
    } // 成功時の終了です。
    std::cout << "[FAIL] " << name << '\n'; // 失敗したテスト名を表示します。
    return false; // 失敗を返します。
} // check関数の終了です。

ReadingRecord createTestRecord() // RecordExporterで使うテスト用ReadingRecordを作ります。
{
    ReadingRecord record;
    record.setDate("260801");
    record.setTitle("人間失格");
    record.setAuthor("太宰治");
    record.setGenre(0, "小説・物語");
    record.setAnswers({ "一言紹介。", "主人公の葉蔵。", "冒頭の一文。", "(スキップ)", "もう会わなくていいと言いたい。", "定期的に読み返したい。" });
    return record;// 完成したテスト用レコードを返します。
} // 関数終了です。

std::vector<QuestionSet> createTestQuestions() // 出力テスト用の質問セットを作ります。
{ // 関数開始です。
    return { QuestionSet("小説・物語", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}) }; // 1ジャンル6問の質問セットを返します。
} // 関数終了です。

fs::path expectedFile() // 通常時に作られるファイルパスを返します。
{ // 関数開始です。
    return fs::path("output") / "260801_人間失格_太宰治.txt"; // 仕様にあるファイル名を組み立てます。
} // 関数終了です。

std::string readBinary(const fs::path& path) // ファイルをバイナリで全部読み込む関数です。
{ // 関数開始です。
    std::ifstream file(path, std::ios::binary); // BOMも確認できるようにバイナリモードで開きます。
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // ファイル全体を文字列として返します。
} // 関数終了です。

} // 無名名前空間の終了です。

bool testExportNewFile() // 新しいファイルを作成できるかテストします。
{ // テスト開始です。
    fs::remove_all("output"); // 前回のテストで残ったoutputフォルダを削除します。
    const ReadingRecord record = createTestRecord(); // テスト用レコードを作ります。
    const auto questions = createTestQuestions(); // テスト用質問セットを作ります。
    const bool result = RecordExporter::exportToFile(record, questions); // ファイル出力処理を実行します。
    const bool exists = fs::exists(expectedFile()); // 期待するファイルが存在するか確認します。
    fs::remove_all("output"); // テスト終了後に出力フォルダを削除します。
    return check(result && exists, "RecordExporter new file"); // 戻り値とファイル存在の両方を確認します。
} // テスト終了です。

bool testOutputDirectoryCreation() // outputフォルダが自動作成されるかテストします。
{ // テスト開始です。
    fs::remove_all("output"); // 最初にoutputフォルダを存在しない状態にします。
    const ReadingRecord record = createTestRecord(); // テスト用レコードを作ります。
    const auto questions = createTestQuestions(); // テスト用質問セットを作ります。
    const bool result = RecordExporter::exportToFile(record, questions); // 出力処理を実行します。
    const bool directoryExists = fs::exists("output") && fs::is_directory("output"); // outputがフォルダとして存在するか確認します。
    fs::remove_all("output"); // テスト終了後にoutputフォルダを削除します。
    return check(result && directoryExists, "RecordExporter output directory creation"); // 出力成功とフォルダ作成を確認します。
} // テスト終了です。

bool testExportContent() // 出力内容に必要な情報が含まれるかテストします。
{ // テスト開始です。
    fs::remove_all("output"); // 前回の出力を削除します。
    const ReadingRecord record = createTestRecord(); // テスト用レコードを作ります。
    const auto questions = createTestQuestions(); // テスト用質問セットを作ります。
    RecordExporter::exportToFile(record, questions); // ファイルを出力します。
    const std::string content = readBinary(expectedFile()); // 出力されたファイルの内容を読み込みます。
    const bool hasTitle = content.find("人間失格") != std::string::npos; // タイトルが含まれるか確認します。
    const bool hasAuthor = content.find("太宰治") != std::string::npos; // 著者名が含まれるか確認します。
    const bool hasDate = content.find("260801") != std::string::npos; // 読了日が含まれるか確認します。
    const bool hasGenre = content.find("小説・物語") != std::string::npos; // ジャンル名が含まれるか確認します。
    const bool hasQuestion = content.find("質問1") != std::string::npos; // 質問文が含まれるか確認します。
    const bool hasAnswer = content.find("主人公の葉蔵。") != std::string::npos; // 回答文が含まれるか確認します。
    fs::remove_all("output"); // テスト終了後に出力フォルダを削除します。
    return check(hasTitle && hasAuthor && hasDate && hasGenre && hasQuestion && hasAnswer, "RecordExporter content"); // 必要な内容がすべて含まれるか確認します。
} // テスト終了です。

bool testUtf8Bom() // UTF-8 BOM付きで出力されるかテストします。
{ // テスト開始です。
    fs::remove_all("output"); // 前回の出力を削除します。
    const ReadingRecord record = createTestRecord(); // テスト用レコードを作ります。
    const auto questions = createTestQuestions(); // テスト用質問セットを作ります。
    RecordExporter::exportToFile(record, questions); // ファイルを出力します。
    const std::string content = readBinary(expectedFile()); // ファイルをバイナリとして読み込みます。
    const bool hasBom = content.size() >= 3 && static_cast<unsigned char>(content[0]) == 0xEF && static_cast<unsigned char>(content[1]) == 0xBB && static_cast<unsigned char>(content[2]) == 0xBF; // 先頭3バイトがUTF-8 BOMか確認します。
    fs::remove_all("output"); // テスト終了後に出力フォルダを削除します。
    return check(hasBom, "RecordExporter UTF-8 BOM"); // BOMが付いているか確認します。
} // テスト終了です。

bool testSanitizeForbiddenCharacters() // ファイル名禁止文字がサニタイズされるかテストします。
{ // テスト開始です。
    fs::remove_all("output"); // 前回の出力を削除します。
    ReadingRecord record; // サニタイズ用のレコードを作ります。
    record.setDate("260801"); // 読了日を設定します。
    record.setTitle("テスト\\/:*?\"<>|"); // Windowsのファイル名で問題になる文字をタイトルに入れます。
    record.setAuthor("太宰治"); // 著者名を設定します。
    record.setGenre(0, "小説・物語"); // ジャンルを設定します。
    record.setAnswers({"回答1", "回答2", "回答3", "回答4"}); // 4問の回答を設定します。
    const auto questions = createTestQuestions(); // 質問セットを作ります。
    const bool result = RecordExporter::exportToFile(record, questions); // ファイル出力を実行します。
    bool foundSanitized = false; // サニタイズ後のファイルが見つかったか保存します。
    if (result && fs::exists("output")) // 出力成功とoutput存在を確認します。
    { // フォルダが存在する場合の開始です。
        for (const auto& entry : fs::directory_iterator("output")) // output内のファイルを1つずつ調べます。
        { // ループ開始です。
            const std::string name = entry.path().filename().string(); // ファイル名だけを文字列として取得します。
            if (name.find("テスト_") != std::string::npos && name.find("太宰治") != std::string::npos) // 禁止文字が_に置き換わった名前か確認します。
            { // サニタイズされたファイルを見つけた場合の開始です。
                foundSanitized = true; // 見つかったことを記録します。
                break; // これ以上調べる必要がないためループを終了します。
            } // 条件成立時の終了です。
        } // ループ終了です。
    } // フォルダ確認の終了です。
    fs::remove_all("output"); // テスト終了後に出力フォルダを削除します。
    return check(foundSanitized, "RecordExporter sanitize forbidden characters"); // サニタイズ後のファイルが作られたか確認します。
} // テスト終了です。

int runRecordExporterTests() // RecordExporterの全テストを実行します。
{ // 関数開始です。
    int failed = 0; // 失敗件数を0で開始します。
    std::cout << "\n--- RecordExporter tests ---\n"; // RecordExporterテストの見出しを表示します。
    failed += !testExportNewFile(); // 新規ファイル作成の結果を集計します。
    failed += !testOutputDirectoryCreation(); // output自動作成の結果を集計します。
    failed += !testExportContent(); // 出力内容の結果を集計します。
    failed += !testUtf8Bom(); // UTF-8 BOMの結果を集計します。
    failed += !testSanitizeForbiddenCharacters(); // サニタイズの結果を集計します。
    return failed; // このファイルの失敗件数を返します。
} // 関数終了です。
