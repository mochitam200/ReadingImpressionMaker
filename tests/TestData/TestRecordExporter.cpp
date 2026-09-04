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
        return record;
    }

    std::vector<QuestionSet> createTestQuestions() // 出力テスト用の質問セットを作ります。
    {
        return { QuestionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}) };
    }

    fs::path expectedFile() // 通常時に作られるファイルパスを返します。
    {
        return fs::path("output") / "260801_人間失格_太宰治.txt";
    }

    std::string readBinary(const fs::path& path) // ファイルをバイナリで全部読み込む関数です。
    {
        std::ifstream file(path, std::ios::binary);
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }

} // 無名名前空間の終了です。

// TC-E-01: 新規出力
bool testExportNewFile()
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    const bool result = RecordExporter::exportToFile(record, questions);
    const bool exists = fs::exists(expectedFile());
    fs::remove_all("output");
    return check(result && exists, "TC-E-01: RecordExporter new file");
}

// TC-E-02: ディレクトリ自動生成
bool testOutputDirectoryCreation()
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    const bool result = RecordExporter::exportToFile(record, questions);
    const bool directoryExists = fs::exists("output") && fs::is_directory("output");
    fs::remove_all("output");
    return check(result && directoryExists, "TC-E-02: RecordExporter output directory creation");
}

// TC-E-03: 出力ファイル内容の整合性
bool testExportContent()
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    RecordExporter::exportToFile(record, questions);
    const std::string content = readBinary(expectedFile());
    const bool hasTitle = content.find("人間失格") != std::string::npos;
    const bool hasAuthor = content.find("太宰治") != std::string::npos;
    const bool hasDate = content.find("260801") != std::string::npos;
    const bool hasGenre = content.find("小説・物語") != std::string::npos;
    const bool hasQuestion = content.find("質問1") != std::string::npos;
    const bool hasAnswer = content.find("主人公の葉蔵。") != std::string::npos;
    fs::remove_all("output");
    return check(hasTitle && hasAuthor && hasDate && hasGenre && hasQuestion && hasAnswer, "TC-E-03: RecordExporter content verification");
}

// TC-E-04: 文字コード (UTF-8 BOM)
bool testUtf8Bom()
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    RecordExporter::exportToFile(record, questions);
    const std::string content = readBinary(expectedFile());
    const bool hasBom = content.size() >= 3 &&
        static_cast<unsigned char>(content[0]) == 0xEF &&
        static_cast<unsigned char>(content[1]) == 0xBB &&
        static_cast<unsigned char>(content[2]) == 0xBF;
    fs::remove_all("output");
    return check(hasBom, "TC-E-04: RecordExporter UTF-8 BOM");
}

// TC-E-05: ファイル名の禁則文字変換
bool testSanitizeForbiddenCharacters()
{
    fs::remove_all("output");
    ReadingRecord record;
    record.setDate("260801");
    record.setTitle("テスト\\/:*?\"<>|");
    record.setAuthor("太宰治");
    record.setGenre(0, "小説・物語");
    record.setAnswers({ "回答1", "回答2", "回答3", "回答4" });
    const auto questions = createTestQuestions();
    const bool result = RecordExporter::exportToFile(record, questions);
    bool foundSanitized = false;
    if (result && fs::exists("output"))
    {
        for (const auto& entry : fs::directory_iterator("output"))
        {
            const std::string name = entry.path().filename().string();
            if (name.find("テスト_") != std::string::npos && name.find("太宰治") != std::string::npos)
            {
                foundSanitized = true;
                break;
            }
        }
    }
    fs::remove_all("output");
    return check(foundSanitized, "TC-E-05: RecordExporter sanitize forbidden characters");
}

// TC-E-06: デフォルト補完値での出力
bool testExportDefaultValues()
{
    fs::remove_all("output");
    ReadingRecord record;
    record.setDate("");        // 空文字 -> 自動補完（本日日付）
    record.setTitle("");       // 空文字 -> Unknown_title
    record.setAuthor("");      // 空文字 -> Unknown_author
    record.setGenre(0, "小説・物語");
    record.setAnswers({ "(スキップ)", "(スキップ)", "(スキップ)", "(スキップ)", "(スキップ)", "(スキップ)" });

    const auto questions = createTestQuestions();
    const bool result = RecordExporter::exportToFile(record, questions);
    bool foundDefaultFile = false;
    if (result && fs::exists("output"))
    {
        for (const auto& entry : fs::directory_iterator("output"))
        {
            const std::string name = entry.path().filename().string();
            if (name.find("Unknown_title_Unknown_author.txt") != std::string::npos)
            {
                foundDefaultFile = true;
                break;
            }
        }
    }
    fs::remove_all("output");
    return check(result && foundDefaultFile, "TC-E-06: RecordExporter default auto-completed values export");
}

int runRecordExporterTests()
{
    int failed = 0;
    std::cout << "\n--- RecordExporter tests ---\n";
    failed += !testExportNewFile();
    failed += !testOutputDirectoryCreation();
    failed += !testExportContent();
    failed += !testUtf8Bom();
    failed += !testSanitizeForbiddenCharacters();
    failed += !testExportDefaultValues();
    return failed;
}