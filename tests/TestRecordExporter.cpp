#include "../include/RecordExporter.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace
{
    ReadingRecord createTestRecord()
    {
        ReadingRecord record;
        record.setDate("260801");
        record.setTitle("人間失格");
        record.setAuthor("太宰治");
        record.setGenre(0, "小説・物語");
        record.setAnswers({ "一言紹介。", "主人公の葉蔵。", "冒頭の一文。", "(スキップ)", "もう会わなくていいと言いたい。", "定期的に読み返したい。" });
        return record;
    }

    std::vector<QuestionSet> createTestQuestions()
    {
        return { QuestionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}) };
    }

    fs::path expectedFile()
    {
        return fs::path("output") / "260801_人間失格_太宰治.txt";
    }

    std::string readBinary(const fs::path& path)
    {
        std::ifstream file(path, std::ios::binary);
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
} // namespace

TEST(RecordExporterTest, TC_E_01_NewFile)
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    const bool result = RecordExporter::exportToFile(record, questions);
    const bool exists = fs::exists(expectedFile());
    fs::remove_all("output");
    EXPECT_TRUE(result);
    EXPECT_TRUE(exists);
}

TEST(RecordExporterTest, TC_E_02_OutputDirectoryCreation)
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    const bool result = RecordExporter::exportToFile(record, questions);
    const bool directoryExists = fs::exists("output") && fs::is_directory("output");
    fs::remove_all("output");
    EXPECT_TRUE(result);
    EXPECT_TRUE(directoryExists);
}

TEST(RecordExporterTest, TC_E_03_ExportContent)
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    RecordExporter::exportToFile(record, questions);
    const std::string content = readBinary(expectedFile());
    fs::remove_all("output");
    EXPECT_NE(content.find("人間失格"), std::string::npos);
    EXPECT_NE(content.find("太宰治"), std::string::npos);
    EXPECT_NE(content.find("260801"), std::string::npos);
    EXPECT_NE(content.find("小説・物語"), std::string::npos);
    EXPECT_NE(content.find("質問1"), std::string::npos);
    EXPECT_NE(content.find("主人公の葉蔵。"), std::string::npos);
}

TEST(RecordExporterTest, TC_E_04_Utf8Bom)
{
    fs::remove_all("output");
    const ReadingRecord record = createTestRecord();
    const auto questions = createTestQuestions();
    RecordExporter::exportToFile(record, questions);
    const std::string content = readBinary(expectedFile());
    fs::remove_all("output");
    ASSERT_GE(content.size(), 3u);
    EXPECT_EQ(static_cast<unsigned char>(content[0]), 0xEF);
    EXPECT_EQ(static_cast<unsigned char>(content[1]), 0xBB);
    EXPECT_EQ(static_cast<unsigned char>(content[2]), 0xBF);
}

TEST(RecordExporterTest, TC_E_05_SanitizeForbiddenCharacters)
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
    EXPECT_TRUE(foundSanitized);
}

TEST(RecordExporterTest, TC_E_06_ExportDefaultValues)
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
    EXPECT_TRUE(result);
    EXPECT_TRUE(foundDefaultFile);
}