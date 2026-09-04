#include "../../include/ConsoleUI.hpp" // テスト対象のConsoleUIを使うために読み込みます。
#include <iostream> // std::cinとテスト結果表示に使います。
#include <sstream> // テスト用の入力ストリームを作るために使います。
#include <string> // 文字列を扱うために使います。
#include <vector> // QuestionSetの一覧を作るために使います。
#include <algorithm> // 判定用アルゴリズムを使います。
#include <cctype> // 数字判定に使います。

class CinRedirect // std::cinを一時的にテスト用文字列へ差し替えるクラスです。
{
private:
    std::streambuf* originalBuffer_;
    std::istringstream inputStream_;

public:
    explicit CinRedirect(const std::string& input) : originalBuffer_(std::cin.rdbuf()), inputStream_(input)
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

    bool check(bool condition, const std::string& name)
    {
        if (condition)
        {
            std::cout << "[PASS] " << name << '\n';
            return true;
        }
        std::cout << "[FAIL] " << name << '\n';
        return false;
    }

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

    std::string makeString50()
    {
        return std::string(50, 'A');
    }

    std::string makeString51()
    {
        return std::string(51, 'A');
    }

} // 無名名前空間

// TC-U-01: 読了日「不明」入力
bool testInputDateUnknown()
{
    CinRedirect input("不明\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "不明", "TC-U-01: inputDate 不明");
}

// TC-U-02: YYMMDD最小値
bool testInputDateValidMin()
{
    CinRedirect input("260101\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "260101", "TC-U-02: inputDate 260101");
}

// TC-U-03: YYMMDD最大値
bool testInputDateValidMax()
{
    CinRedirect input("261231\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "261231", "TC-U-03: inputDate 261231");
}

// TC-U-04: 月日の範囲外→再入力
bool testInputDateInvalidMonthAndDay()
{
    CinRedirect input("260001\n261301\n260832\n260801\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "260801", "TC-U-04: inputDate invalid month/day then valid");
}

// TC-U-05: YYMM形式最小値
bool testInputDateValidMonth()
{
    CinRedirect input("2601\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "2601", "TC-U-05: inputDate 2601");
}

// TC-U-06: YYMM形式最大値
bool testInputDateValidMonthMax()
{
    CinRedirect input("2612\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "2612", "TC-U-06: inputDate 2612");
}

// TC-U-07: YYMM不正形式→再入力
bool testInputDateInvalidMonthFormat()
{
    CinRedirect input("2600\n2613\n2601\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "2601", "TC-U-07: inputDate invalid YYMM then valid");
}

// TC-U-08: 4季節入力（春夏秋冬）
bool testInputDateSeasons()
{
    bool ok = true;
    { CinRedirect input("26春\n"); ok = ok && ConsoleUI::inputDate() == "26春"; }
    { CinRedirect input("26夏\n"); ok = ok && ConsoleUI::inputDate() == "26夏"; }
    { CinRedirect input("26秋\n"); ok = ok && ConsoleUI::inputDate() == "26秋"; }
    { CinRedirect input("26冬\n"); ok = ok && ConsoleUI::inputDate() == "26冬"; }
    return check(ok, "TC-U-08: inputDate four seasons");
}

// TC-U-09: 不正季節表記→再入力
bool testInputDateInvalidSeason()
{
    CinRedirect input("26初夏\n26Spring\n26春\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "26春", "TC-U-09: inputDate invalid season then valid");
}

// TC-U-10: 空入力（Enterのみ）自動補完
bool testInputDateEmptyAuto()
{
    CinRedirect input("\n");
    const std::string result = ConsoleUI::inputDate();
    const bool validTodayFormat = (result.length() == 6) &&
        std::all_of(result.begin(), result.end(), ::isdigit) &&
        (std::stoi(result.substr(2, 2)) >= 1 && std::stoi(result.substr(2, 2)) <= 12) &&
        (std::stoi(result.substr(4, 2)) >= 1 && std::stoi(result.substr(4, 2)) <= 31);
    return check(validTodayFormat, "TC-U-10: inputDate empty auto-complete");
}

// TC-U-11: 未対応フォーマット→再入力
bool testInputDateDifferentFormat()
{
    CinRedirect input("2026/08/01\n260801\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "260801", "TC-U-11: inputDate different format then valid");
}

// TC-U-12: 通常のタイトル
bool testInputTitleValid()
{
    CinRedirect input("人間失格\n");
    const std::string result = ConsoleUI::inputTitle();
    return check(result == "人間失格", "TC-U-12: inputTitle normal");
}

// TC-U-13: 中断コマンド誤認知防止
bool testInputTitleQ84()
{
    CinRedirect input("1Q84\n");
    const std::string result = ConsoleUI::inputTitle();
    return check(result == "1Q84", "TC-U-13: inputTitle 1Q84");
}

// TC-U-14: 境界値 50文字タイトル
bool testInputTitle50Characters()
{
    const std::string title = makeString50();
    CinRedirect input(title + "\n");
    const std::string result = ConsoleUI::inputTitle();
    return check(result == title, "TC-U-14: inputTitle 50 characters");
}

// TC-U-15: 境界値 51文字タイトル→再入力
bool testInputTitle51Characters()
{
    const std::string title51 = makeString51();
    CinRedirect input(title51 + "\n人間失格\n");
    const std::string result = ConsoleUI::inputTitle();
    return check(result == "人間失格", "TC-U-15: inputTitle 51 characters then valid");
}

// TC-U-16: 空入力（Enterのみ）自動補完
bool testInputTitleEmptyAuto()
{
    CinRedirect input("\n");
    const std::string result = ConsoleUI::inputTitle();
    return check(result == "Unknown_title", "TC-U-16: inputTitle empty auto-complete");
}

// TC-U-17: スペースのみ入力自動補完
bool testInputTitleSpacesAuto()
{
    CinRedirect input("   \n");
    const std::string result = ConsoleUI::inputTitle();
    return check(result == "Unknown_title", "TC-U-17: inputTitle spaces auto-complete");
}

// TC-U-18: 通常の著者名
bool testInputAuthorValid()
{
    CinRedirect input("太宰治\n");
    const std::string result = ConsoleUI::inputAuthor();
    return check(result == "太宰治", "TC-U-18: inputAuthor normal");
}

// TC-U-19: 空入力（Enterのみ）自動補完
bool testInputAuthorEmptyAuto()
{
    CinRedirect input("\n");
    const std::string result = ConsoleUI::inputAuthor();
    return check(result == "Unknown_author", "TC-U-19: inputAuthor empty auto-complete");
}

// TC-U-20: 境界値 50文字著者名
bool testInputAuthor50Characters()
{
    const std::string author = makeString50();
    CinRedirect input(author + "\n");
    const std::string result = ConsoleUI::inputAuthor();
    return check(result == author, "TC-U-20: inputAuthor 50 characters");
}

// TC-U-21: 境界値 51文字著者名→再入力
bool testInputAuthor51Characters()
{
    const std::string author51 = makeString51();
    CinRedirect input(author51 + "\n太宰治\n");
    const std::string result = ConsoleUI::inputAuthor();
    return check(result == "太宰治", "TC-U-21: inputAuthor 51 characters then valid");
}

// TC-U-22: ジャンル1を選択・確定
bool testSelectGenreFirst()
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("1\ny\n");
    const int result = ConsoleUI::selectGenre(questions);
    return check(result == 0, "TC-U-22: selectGenre 1");
}

// TC-U-23: ジャンル5を選択・確定
bool testSelectGenreLast()
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("5\ny\n");
    const int result = ConsoleUI::selectGenre(questions);
    return check(result == 4, "TC-U-23: selectGenre 5");
}

// TC-U-24: 不正範囲入力→再選択
bool testSelectGenreInvalidThenValid()
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("0\n6\na\n1\ny\n");
    const int result = ConsoleUI::selectGenre(questions);
    return check(result == 0, "TC-U-24: selectGenre invalid then valid");
}

// TC-U-25: キャンセル後の再選択
bool testSelectGenreCancelThenSelect()
{
    const auto questions = createTestQuestionSets();
    CinRedirect input("1\nn\n2\ny\n");
    const int result = ConsoleUI::selectGenre(questions);
    return check(result == 1, "TC-U-25: selectGenre cancel then select 2");
}

// TC-U-26: 通常回答（1行ずつ）
bool testAskQuestionsOneLine()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("回答1\n\n回答2\n\n回答3\n\n回答4\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    return check(answers.size() == 6 && answers[0] == "回答1" && answers[5] == "回答6", "TC-U-26: askQuestions one line");
}

// TC-U-27: スキップ処理 (skip 入力)
bool testAskQuestionsSkip()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("skip\nskip\nskip\nskip\nskip\nskip\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    return check(answers.size() == 6 && answers[0] == "(スキップ)" && answers[5] == "(スキップ)", "TC-U-27: askQuestions all skip command");
}

// TC-U-28: スキップ処理 (未入力Enter)
bool testAskQuestionsSkipEmpty()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("\n\n\n\n\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    return check(answers.size() == 6 && answers[0] == "(スキップ)" && answers[5] == "(スキップ)", "TC-U-28: askQuestions all skip by empty Enter");
}

// TC-U-29: 複数行回答（改行エスケープ \）
bool testAskQuestionsMultilineEscape()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("行1\\\n行2\n\n回答2\n\n回答3\n\n回答4\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    return check(answers.size() == 6 && answers[0] == "行1\n行2", "TC-U-29: askQuestions multiline escape");
}

// TC-U-30: skip 部分一致の誤認防止
bool testAskQuestionsSkipWordIsNotPartialMatch()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("skipper\n\nskipです\n\n通常回答\n\nskip\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    const bool first = answers.size() >= 1 && answers[0] == "skipper";
    const bool second = answers.size() >= 2 && answers[1] == "skipです";
    const bool fourth = answers.size() >= 4 && answers[3] == "(スキップ)";
    return check(first && second && fourth, "TC-U-30: askQuestions skip partial match");
}

int runConsoleUITests()
{
    int failed = 0;
    std::cout << "\n--- ConsoleUI tests ---\n";
    failed += !testInputDateUnknown();
    failed += !testInputDateValidMin();
    failed += !testInputDateValidMax();
    failed += !testInputDateInvalidMonthAndDay();
    failed += !testInputDateValidMonth();
    failed += !testInputDateValidMonthMax();
    failed += !testInputDateInvalidMonthFormat();
    failed += !testInputDateSeasons();
    failed += !testInputDateInvalidSeason();
    failed += !testInputDateEmptyAuto();
    failed += !testInputDateDifferentFormat();
    failed += !testInputTitleValid();
    failed += !testInputTitleQ84();
    failed += !testInputTitle50Characters();
    failed += !testInputTitle51Characters();
    failed += !testInputTitleEmptyAuto();
    failed += !testInputTitleSpacesAuto();
    failed += !testInputAuthorValid();
    failed += !testInputAuthorEmptyAuto();
    failed += !testInputAuthor50Characters();
    failed += !testInputAuthor51Characters();
    failed += !testSelectGenreFirst();
    failed += !testSelectGenreLast();
    failed += !testSelectGenreInvalidThenValid();
    failed += !testSelectGenreCancelThenSelect();
    failed += !testAskQuestionsOneLine();
    failed += !testAskQuestionsSkip();
    failed += !testAskQuestionsSkipEmpty();
    failed += !testAskQuestionsMultilineEscape();
    failed += !testAskQuestionsSkipWordIsNotPartialMatch();
    return failed;
}