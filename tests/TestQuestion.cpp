#include "../include/Question.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// ---- QuestionSet ----

TEST(QuestionSetTest, TC_Q_01_Constructor)
{
    const std::vector<std::string> questions{ "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" };
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", questions);
    EXPECT_EQ(questionSet.getGenreName(), "小説・物語");
    EXPECT_EQ(questionSet.getGenreDescription(), "ストーリーや登場人物の心情を楽しむ本");
    EXPECT_EQ(questionSet.getQuestions(), questions);
}

TEST(QuestionSetTest, TC_Q_02_GetGenreName)
{
    const QuestionSet questionSet("実用書", "日常生活ですぐ使えるノウハウを提供する本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    EXPECT_EQ(questionSet.getGenreName(), "実用書");
}

TEST(QuestionSetTest, TC_Q_03_GetGenreDescription)
{
    const QuestionSet questionSet("実用書", "日常生活ですぐ使えるノウハウを提供する本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    EXPECT_EQ(questionSet.getGenreDescription(), "日常生活ですぐ使えるノウハウを提供する本");
}

TEST(QuestionSetTest, TC_Q_04_GetQuestions)
{
    const std::vector<std::string> questions{ "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" };
    const QuestionSet questionSet("実用書", "日常生活ですぐ使えるノウハウを提供する本", questions);
    EXPECT_EQ(questionSet.getQuestions(), questions);
}

TEST(QuestionSetTest, TC_Q_05_GetQuestionFirst)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    EXPECT_EQ(questionSet.getQuestion(0), "質問1");
}

TEST(QuestionSetTest, TC_Q_06_GetQuestionLast)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    EXPECT_EQ(questionSet.getQuestion(5), "質問6");
}

TEST(QuestionSetTest, TC_Q_07_GetQuestionCount)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    EXPECT_EQ(questionSet.getQuestionCount(), 6);
}

// ---- ReadingRecord ----

TEST(ReadingRecordTest, TC_R_01_Constructor)
{
    ReadingRecord record;
    (void)record;
    // コンストラクタが例外なく動作することを確認
    SUCCEED();
}

TEST(ReadingRecordTest, TC_R_02_SetDateUnknown)
{
    ReadingRecord record;
    EXPECT_TRUE(record.setDate("不明"));
    EXPECT_EQ(record.getDate(), "不明");
}

TEST(ReadingRecordTest, TC_R_03_SetDateValid)
{
    ReadingRecord record;
    EXPECT_TRUE(record.setDate("260801"));
    EXPECT_EQ(record.getDate(), "260801");
}

TEST(ReadingRecordTest, TC_R_04_SetDateInvalid)
{
    ReadingRecord record;
    EXPECT_FALSE(record.setDate("26080"));
}

TEST(ReadingRecordTest, TC_R_05_SetDateEmptyAutoComplete)
{
    ReadingRecord record;
    EXPECT_TRUE(record.setDate(""));
    const std::string d = record.getDate();
    ASSERT_EQ(d.length(), 6u);
    EXPECT_TRUE(std::all_of(d.begin(), d.end(), ::isdigit));
    EXPECT_GE(std::stoi(d.substr(2, 2)), 1);
    EXPECT_LE(std::stoi(d.substr(2, 2)), 12);
    EXPECT_GE(std::stoi(d.substr(4, 2)), 1);
    EXPECT_LE(std::stoi(d.substr(4, 2)), 31);
}

TEST(ReadingRecordTest, TC_R_06_SetTitleValid)
{
    ReadingRecord record;
    EXPECT_TRUE(record.setTitle("人間失格"));
    EXPECT_EQ(record.getTitle(), "人間失格");
}

TEST(ReadingRecordTest, TC_R_07_SetTitleEmptyAutoComplete)
{
    ReadingRecord record;
    EXPECT_TRUE(record.setTitle(""));
    EXPECT_EQ(record.getTitle(), "Unknown_title");
}

TEST(ReadingRecordTest, TC_R_08_SetTitle50Characters)
{
    ReadingRecord record;
    const std::string title(50, 'A');
    EXPECT_TRUE(record.setTitle(title));
    EXPECT_EQ(record.getTitle(), title);
}

TEST(ReadingRecordTest, TC_R_09_SetTitle51CharactersRejected)
{
    ReadingRecord record;
    const std::string title(51, 'A');
    EXPECT_FALSE(record.setTitle(title));
}

TEST(ReadingRecordTest, TC_R_10_SetAuthorValid)
{
    ReadingRecord record;
    EXPECT_TRUE(record.setAuthor("太宰治"));
    EXPECT_EQ(record.getAuthor(), "太宰治");
}

TEST(ReadingRecordTest, TC_R_11_SetAuthorEmptyAutoComplete)
{
    ReadingRecord record;
    EXPECT_TRUE(record.setAuthor(""));
    EXPECT_EQ(record.getAuthor(), "Unknown_author");
}

TEST(ReadingRecordTest, TC_R_12_SetAuthor50Characters)
{
    ReadingRecord record;
    const std::string author(50, 'A');
    EXPECT_TRUE(record.setAuthor(author));
    EXPECT_EQ(record.getAuthor(), author);
}

TEST(ReadingRecordTest, TC_R_13_SetAuthor51CharactersRejected)
{
    ReadingRecord record;
    const std::string author(51, 'A');
    EXPECT_FALSE(record.setAuthor(author));
}

TEST(ReadingRecordTest, TC_R_14_SetGenre)
{
    ReadingRecord record;
    record.setGenre(0, "小説・物語");
    EXPECT_EQ(record.getGenreIndex(), 0);
    EXPECT_EQ(record.getGenreName(), "小説・物語");
}

TEST(ReadingRecordTest, TC_R_15_SetAnswers)
{
    ReadingRecord record;
    const std::vector<std::string> answers{ "回答1", "回答2", "回答3", "(スキップ)", "回答5", "回答6" };
    record.setAnswers(answers);
    EXPECT_EQ(record.getAnswers(), answers);
}

TEST(ReadingRecordTest, TC_R_16_SetAnswer)
{
    ReadingRecord record;
    const std::vector<std::string> answers{ "回答1", "回答2", "回答3", "回答4", "回答5", "回答6" };
    record.setAnswers(answers);
    record.setAnswer(1, "修正後の回答");
    EXPECT_EQ(record.getAnswer(1), "修正後の回答");
    EXPECT_EQ(record.getAnswer(0), "回答1");
    EXPECT_EQ(record.getAnswer(2), "回答3");
    EXPECT_EQ(record.getAnswer(3), "回答4");
    EXPECT_EQ(record.getAnswer(4), "回答5");
    EXPECT_EQ(record.getAnswer(5), "回答6");
}