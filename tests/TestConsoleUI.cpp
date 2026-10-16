#include "../include/ConsoleUI.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// std::cin をテスト用文字列へ差し替えるRAIIクラス
class CinRedirect
{
private:
    std::streambuf* originalBuffer_;
    std::istringstream inputStream_;
public:
    explicit CinRedirect(const std::string& input)
        : originalBuffer_(std::cin.rdbuf()), inputStream_(input)
    {
        std::cin.rdbuf(inputStream_.rdbuf());
    }
    ~CinRedirect()
    {
        std::cin.rdbuf(originalBuffer_);
    }
};

namespace
{
    std::vector<QuestionSet> createTestQuestionSets()
    {
        return {
            QuestionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
            QuestionSet("実用書", "日常生活ですぐ使えるノウハウを提供する本", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
            QuestionSet("ビジネス・自己啓発", "ビジネススキルや自己成長を直接扱う本", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
            QuestionSet("専門書・学習書", "特定の分野について専門的・学術的に深く追求するための本", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
            QuestionSet("ノンフィクション・伝記", "事実に基づいた記録や人物の生涯を描いた本", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"})
        };
    }
} // namespace

// ---- inputDate ----

TEST(ConsoleUITest, TC_U_01_InputDateUnknown)
{
    CinRedirect input("不明\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "不明");
}

TEST(ConsoleUITest, TC_U_02_InputDateValidMin)
{
    CinRedirect input("260101\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "260101");
}

TEST(ConsoleUITest, TC_U_03_InputDateValidMax)
{
    CinRedirect input("261231\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "261231");
}

TEST(ConsoleUITest, TC_U_04_InputDateInvalidMonthAndDay)
{
    CinRedirect input("260001\n261301\n260832\n260801\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "260801");
}

TEST(ConsoleUITest, TC_U_05_InputDateValidMonth)
{
    CinRedirect input("2601\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "2601");
}

TEST(ConsoleUITest, TC_U_06_InputDateValidMonthMax)
{
    CinRedirect input("2612\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "2612");
}

TEST(ConsoleUITest, TC_U_07_InputDateInvalidMonthFormat)
{
    CinRedirect input("2600\n2613\n2601\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "2601");
}

TEST(ConsoleUITest, TC_U_08_InputDateSeasons)
{
    { CinRedirect input("26春\n"); EXPECT_EQ(ConsoleUI::inputDate(), "26春"); }
    { CinRedirect input("26夏\n"); EXPECT_EQ(ConsoleUI::inputDate(), "26夏"); }
    { CinRedirect input("26秋\n"); EXPECT_EQ(ConsoleUI::inputDate(), "26秋"); }
    { CinRedirect input("26冬\n"); EXPECT_EQ(ConsoleUI::inputDate(), "26冬"); }
}

TEST(ConsoleUITest, TC_U_09_InputDateInvalidSeason)
{
    CinRedirect input("26初夏\n26Spring\n26春\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "26春");
}

TEST(ConsoleUITest, TC_U_10_InputDateEmptyAutoComplete)
{
    CinRedirect input("\n");
    const std::string result = ConsoleUI::inputDate();
    ASSERT_EQ(result.length(), 6u);
    EXPECT_TRUE(std::all_of(result.begin(), result.end(), ::isdigit));
    EXPECT_GE(std::stoi(result.substr(2, 2)), 1);
    EXPECT_LE(std::stoi(result.substr(2, 2)), 12);
    EXPECT_GE(std::stoi(result.substr(4, 2)), 1);
    EXPECT_LE(std::stoi(result.substr(4, 2)), 31);
}

TEST(ConsoleUITest, TC_U_11_InputDateDifferentFormat)
{
    CinRedirect input("2026/08/01\n260801\n");
    EXPECT_EQ(ConsoleUI::inputDate(), "260801");
}

// ---- inputTitle ----

TEST(ConsoleUITest, TC_U_12_InputTitleValid)
{
    CinRedirect input("人間失格\n");
    EXPECT_EQ(ConsoleUI::inputTitle(), "人間失格");
}

TEST(ConsoleUITest, TC_U_13_InputTitleQ84)
{
    CinRedirect input("1Q84\n");
    EXPECT_EQ(ConsoleUI::inputTitle(), "1Q84");
}

TEST(ConsoleUITest, TC_U_14_InputTitle50Characters)
{
    const std::string title(50, 'A');
    CinRedirect input(title + "\n");
    EXPECT_EQ(ConsoleUI::inputTitle(), title);
}

TEST(ConsoleUITest, TC_U_15_InputTitle51Characters)
{
    const std::string title51(51, 'A');
    CinRedirect input(title51 + "\n人間失格\n");
    EXPECT_EQ(ConsoleUI::inputTitle(), "人間失格");
}

TEST(ConsoleUITest, TC_U_16_InputTitleEmptyAutoComplete)
{
    CinRedirect input("\n");
    EXPECT_EQ(ConsoleUI::inputTitle(), "Unknown_title");
}

TEST(ConsoleUITest, TC_U_17_InputTitleSpacesAutoComplete)
{
    CinRedirect input("   \n");
    EXPECT_EQ(ConsoleUI::inputTitle(), "Unknown_title");
}

// ---- inputAuthor ----

TEST(ConsoleUITest, TC_U_18_InputAuthorValid)
{
    CinRedirect input("太宰治\n");
    EXPECT_EQ(ConsoleUI::inputAuthor(), "太宰治");
}

TEST(ConsoleUITest, TC_U_19_InputAuthorEmptyAutoComplete)
{
    CinRedirect input("\n");
    EXPECT_EQ(ConsoleUI::inputAuthor(), "Unknown_author");
}

TEST(ConsoleUITest, TC_U_20_InputAuthor50Characters)
{
    const std::string author(50, 'A');
    CinRedirect input(author + "\n");
    EXPECT_EQ(ConsoleUI::inputAuthor(), author);
}

TEST(ConsoleUITest, TC_U_21_InputAuthor51Characters)
{
    const std::string author51(51, 'A');
    CinRedirect input(author51 + "\n太宰治\n");
    EXPECT_EQ(ConsoleUI::inputAuthor(), "太宰治");
}

// ---- selectGenre ----

TEST(ConsoleUITest, TC_U_22_SelectGenreFirst)
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("1\ny\n");
    EXPECT_EQ(ConsoleUI::selectGenre(questions), 0);
}

TEST(ConsoleUITest, TC_U_23_SelectGenreLast)
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("5\ny\n");
    EXPECT_EQ(ConsoleUI::selectGenre(questions), 4);
}

TEST(ConsoleUITest, TC_U_24_SelectGenreInvalidThenValid)
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("0\n6\na\n1\ny\n");
    EXPECT_EQ(ConsoleUI::selectGenre(questions), 0);
}

TEST(ConsoleUITest, TC_U_25_SelectGenreCancelThenSelect)
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("1\nn\n2\ny\n");
    EXPECT_EQ(ConsoleUI::selectGenre(questions), 1);
}

// ---- askQuestions ----

TEST(ConsoleUITest, TC_U_26_AskQuestionsOneLine)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("回答1\n\n回答2\n\n回答3\n\n回答4\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    ASSERT_EQ(answers.size(), 6u);
    EXPECT_EQ(answers[0], "回答1");
    EXPECT_EQ(answers[5], "回答6");
}

TEST(ConsoleUITest, TC_U_27_AskQuestionsSkip)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("skip\nskip\nskip\nskip\nskip\nskip\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    ASSERT_EQ(answers.size(), 6u);
    EXPECT_EQ(answers[0], "(スキップ)");
    EXPECT_EQ(answers[5], "(スキップ)");
}

TEST(ConsoleUITest, TC_U_28_AskQuestionsSkipEmpty)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("\n\n\n\n\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    ASSERT_EQ(answers.size(), 6u);
    EXPECT_EQ(answers[0], "(スキップ)");
    EXPECT_EQ(answers[5], "(スキップ)");
}

TEST(ConsoleUITest, TC_U_29_AskQuestionsMultilineEscape)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("行1\\\n行2\n\n回答2\n\n回答3\n\n回答4\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    ASSERT_EQ(answers.size(), 6u);
    EXPECT_EQ(answers[0], "行1\n行2");
}

TEST(ConsoleUITest, TC_U_30_AskQuestionsSkipPartialMatch)
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("skipper\n\nskipです\n\n通常回答\n\nskip\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    ASSERT_GE(answers.size(), 4u);
    EXPECT_EQ(answers[0], "skipper");
    EXPECT_EQ(answers[1], "skipです");
    EXPECT_EQ(answers[3], "(スキップ)");
}