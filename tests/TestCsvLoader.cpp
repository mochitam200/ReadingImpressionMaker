#include "../include/CsvLoader.hpp"
#include <gtest/gtest.h>
#include <cstdio>
#include <fstream>
#include <string>

namespace
{
    void writeTextFile(const std::string& filename, const std::string& text)
    {
        std::ofstream file(filename, std::ios::binary);
        file << text;
    }

    std::string makeValidCsv(const std::string& newline = "\n")
    {
        std::string csv;
        csv += "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,1,一言で紹介するならどんな本ですか" + newline;
        csv += "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,2,一番印象に残った登場人物は誰？（好き・嫌い・気になった理由も）" + newline;
        csv += "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,3,一番息をのんだ「名シーン」や「名台詞」はどこ？" + newline;
        csv += "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,4,この本を読んでいる時、心に一番残った感情は？" + newline;
        csv += "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,5,もし自分が物語の中に入れたら、どの場面で主人公にどんな声をかけたい？" + newline;
        csv += "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,6,最後に何かあればご自由にお書きください" + newline;

        csv += "2,実用書,日常生活ですぐ使えるノウハウを提供する本,1,一言で紹介するならどんな本ですか" + newline;
        csv += "2,実用書,日常生活ですぐ使えるノウハウを提供する本,2,「これはすぐ使える！」と膝を打ったテクニックやノウハウは？" + newline;
        csv += "2,実用書,日常生活ですぐ使えるノウハウを提供する本,3,読んだ今日・明日から、具体的に試してみたい「小さな一歩（ToDo）」は？" + newline;
        csv += "2,実用書,日常生活ですぐ使えるノウハウを提供する本,4,逆に「これは今の自分には合わない・不要だ」と思った部分は？" + newline;
        csv += "2,実用書,日常生活ですぐ使えるノウハウを提供する本,5,この本の要点を、まだ読んでいない友達に一言で教えるなら？" + newline;
        csv += "2,実用書,日常生活ですぐ使えるノウハウを提供する本,6,最後に何かあればご自由にお書きください" + newline;

        csv += "3,ビジネス・自己啓発,ビジネススキルや自己成長を直接扱う本,1,一言で紹介するならどんな本ですか" + newline;
        csv += "3,ビジネス・自己啓発,ビジネススキルや自己成長を直接扱う本,2,ページをめくる手が止まった「ハイライトしたい一言（フレーズ）」は？" + newline;
        csv += "3,ビジネス・自己啓発,ビジネススキルや自己成長を直接扱う本,3,今までの自分の「常識」や「言い訳」を打ち砕かれた部分はどこ？" + newline;
        csv += "3,ビジネス・自己啓発,ビジネススキルや自己成長を直接扱う本,4,この本を読む前と読んだ後で、自分の「行動」や「選択」はどう変わりそう？" + newline;
        csv += "3,ビジネス・自己啓発,ビジネススキルや自己成長を直接扱う本,5,1ヶ月後の自分が忘れないように、スマホの待受けにしたいレベルの「教訓」は？" + newline;
        csv += "3,ビジネス・自己啓発,ビジネススキルや自己成長を直接扱う本,6,最後に何かあればご自由にお書きください" + newline;

        csv += "4,専門書・学習書,特定の分野について専門的・学術的に深く追求するための本,1,一言で紹介するならどんな本ですか" + newline;
        csv += "4,専門書・学習書,特定の分野について専門的・学術的に深く追求するための本,2,この本を読む前と後で、一番「解像度が上がった」概念やテーマは？" + newline;
        csv += "4,専門書・学習書,特定の分野について専門的・学術的に深く追求するための本,3,専門用語や理論の中で、「なるほど、そういうことか！」とスッキリした部分は？" + newline;
        csv += "4,専門書・学習書,特定の分野について専門的・学術的に深く追求するための本,4,読んでいて「ここをもっと調べたい」「まだ謎が残る」と思った課題は？" + newline;
        csv += "4,専門書・学習書,特定の分野について専門的・学術的に深く追求するための本,5,この知識を手に入れたことで、次に読んでみたい本や挑戦したいステップは？" + newline;
        csv += "4,専門書・学習書,特定の分野について専門的・学術的に深く追求するための本,6,最後に何かあればご自由にお書きください" + newline;

        csv += "5,ノンフィクション・伝記,事実に基づいた記録や人物の生涯を描いた本,1,一言で紹介するならどんな本ですか" + newline;
        csv += "5,ノンフィクション・伝記,事実に基づいた記録や人物の生涯を描いた本,2,主人公（または登場人物）の「生き方」や「決断」で、一番震えたシーンは？" + newline;
        csv += "5,ノンフィクション・伝記,事実に基づいた記録や人物の生涯を描いた本,3,自分がもし同じ時代・同じ立場にいたら、同じ決断ができたと思う？" + newline;
        csv += "5,ノンフィクション・伝記,事実に基づいた記録や人物の生涯を描いた本,4,この本を読む前と後で、ニュースや世の中に対する「見え方」はどう変わった？" + newline;
        csv += "5,ノンフィクション・伝記,事実に基づいた記録や人物の生涯を描いた本,5,著者の情熱や生き方から、自分の人生に「感染」させたいエネルギーや姿勢は？" + newline;
        csv += "5,ノンフィクション・伝記,事実に基づいた記録や人物の生涯を描いた本,6,最後に何かあればご自由にお書きください" + newline;
        return csv;
    }
} // namespace

TEST(CsvLoaderTest, TC_C_01_ValidLF)
{
    const std::string filename = "test_valid_lf.csv";
    writeTextFile(filename, makeValidCsv("\n"));
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    ASSERT_EQ(result.size(), 5u);
    EXPECT_EQ(result[0].getGenreName(), "小説・物語");
    EXPECT_EQ(result[0].getGenreDescription(), "ストーリーや登場人物の心情を楽しむ本");
    EXPECT_EQ(result[0].getQuestionCount(), 6);
    EXPECT_EQ(result[4].getQuestionCount(), 6);
}

TEST(CsvLoaderTest, TC_C_02_ValidCRLF)
{
    const std::string filename = "test_valid_crlf.csv";
    writeTextFile(filename, makeValidCsv("\r\n"));
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    ASSERT_EQ(result.size(), 5u);
    EXPECT_EQ(result[0].getQuestionCount(), 6);
    EXPECT_EQ(result[4].getQuestionCount(), 6);
}

TEST(CsvLoaderTest, TC_C_03_ValidBOM)
{
    const std::string filename = "test_valid_bom.csv";
    writeTextFile(filename, std::string("\xEF\xBB\xBF") + makeValidCsv());
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    ASSERT_EQ(result.size(), 5u);
    EXPECT_EQ(result[0].getGenreName(), "小説・物語");
    EXPECT_EQ(result[0].getGenreDescription(), "ストーリーや登場人物の心情を楽しむ本");
}

TEST(CsvLoaderTest, TC_C_04_FileNotFound)
{
    std::remove("test_not_found.csv");
    const auto result = CsvLoader::load("test_not_found.csv");
    EXPECT_TRUE(result.empty());
}

TEST(CsvLoaderTest, TC_C_05_EmptyCsv)
{
    const std::string filename = "test_empty.csv";
    writeTextFile(filename, "");
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    EXPECT_TRUE(result.empty());
}

TEST(CsvLoaderTest, TC_C_06_LessThan30Rows)
{
    const std::string filename = "test_29_rows.csv";
    std::string csv;
    for (int i = 0; i < 29; ++i)
    {
        csv += "1,小説・物語,ストーリーや登場人物の心情を楽しむ本," + std::to_string(i + 1) + ",質問\n";
    }
    writeTextFile(filename, csv);
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    EXPECT_TRUE(result.empty());
}

TEST(CsvLoaderTest, TC_C_07_TooFewColumns)
{
    const std::string filename = "test_few_columns.csv";
    writeTextFile(filename, "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,1\n");
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    EXPECT_TRUE(result.empty());
}

TEST(CsvLoaderTest, TC_C_08_TooManyColumns)
{
    const std::string filename = "test_many_columns.csv";
    writeTextFile(filename, "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,1,質問文,余分な列\n");
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    EXPECT_TRUE(result.empty());
}

TEST(CsvLoaderTest, TC_C_09_InvalidGenreNumber)
{
    const std::string filename = "test_invalid_genre.csv";
    writeTextFile(filename, "A,小説・物語,ストーリーや登場人物の心情を楽しむ本,1,質問文\n");
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    EXPECT_TRUE(result.empty());
}

TEST(CsvLoaderTest, TC_C_10_InvalidQuestionNumber)
{
    const std::string filename = "test_invalid_question.csv";
    writeTextFile(filename, "1,小説・物語,ストーリーや登場人物の心情を楽しむ本,A,質問文\n");
    const auto result = CsvLoader::load(filename);
    std::remove(filename.c_str());
    EXPECT_TRUE(result.empty());
}