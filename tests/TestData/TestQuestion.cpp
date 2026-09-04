#include "../../include/Question.hpp" // テスト対象のQuestionSetとReadingRecordを使うために読み込みます。
#include <iostream> // テスト結果を画面へ表示するために使います。
#include <string> // 文字列を扱うために使います。
#include <vector> // 質問と回答の配列を扱うために使います。
#include <algorithm> // 文字列判定などに使います。
#include <cctype> // 数字判定に使います。

namespace // このファイルだけで使う補助関数を名前空間で囲みます。
{ // 無名名前空間の開始です。

    bool check(bool condition, const std::string& name) // 条件を確認して結果を表示する共通関数です。
    { // check関数の開始です。
        if (condition) // 期待した結果になっているか確認します。
        { // 正常だった場合の開始です。
            std::cout << "[PASS] " << name << '\n'; // テスト成功を表示します。
            return true; // 成功したことを呼び出し側へ返します。
        } // 正常だった場合の終了です。

        std::cout << "[FAIL] " << name << '\n'; // テスト失敗を表示します。
        return false; // 失敗したことを呼び出し側へ返します。
    } // check関数の終了です。

} // 無名名前空間の終了です。

// TC-Q-01: コンストラクタ検証
bool testQuestionSetConstructor()
{
    const std::vector<std::string> questions{ "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" };
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", questions);
    return check(questionSet.getGenreName() == "小説・物語" &&
        questionSet.getGenreDescription() == "ストーリーや登場人物の心情を楽しむ本" &&
        questionSet.getQuestions() == questions,
        "TC-Q-01: QuestionSet constructor");
}

// TC-Q-02: ジャンル名取得
bool testQuestionSetGetGenreName()
{
    const QuestionSet questionSet("実用書", "日常生活ですぐ使えるノウハウを提供する本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    return check(questionSet.getGenreName() == "実用書", "TC-Q-02: QuestionSet getGenreName");
}

// TC-Q-03: ジャンル説明取得
bool testQuestionSetGetGenreDescription()
{
    const QuestionSet questionSet("実用書", "日常生活ですぐ使えるノウハウを提供する本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    return check(questionSet.getGenreDescription() == "日常生活ですぐ使えるノウハウを提供する本", "TC-Q-03: QuestionSet getGenreDescription");
}

// TC-Q-04: 質問一覧取得
bool testQuestionSetGetQuestions()
{
    const std::vector<std::string> questions{ "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" };
    const QuestionSet questionSet("実用書", "日常生活ですぐ使えるノウハウを提供する本", questions);
    return check(questionSet.getQuestions() == questions, "TC-Q-04: QuestionSet getQuestions");
}

// TC-Q-05: 0番目の質問取得
bool testQuestionSetGetQuestionFirst()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    return check(questionSet.getQuestion(0) == "質問1", "TC-Q-05: QuestionSet getQuestion first");
}

// TC-Q-06: 5番目の質問取得
bool testQuestionSetGetQuestionLast()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    return check(questionSet.getQuestion(5) == "質問6", "TC-Q-06: QuestionSet getQuestion last");
}

// TC-Q-07: 質問数取得
bool testQuestionSetGetQuestionCount()
{
    const QuestionSet questionSet("小説・物語", "ストーリーや登場人物の心情を楽しむ本", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    return check(questionSet.getQuestionCount() == 6, "TC-Q-07: QuestionSet getQuestionCount");
}

// TC-R-01: コンストラクタ検証
bool testReadingRecordConstructor()
{
    ReadingRecord record;
    (void)record;
    return check(true, "TC-R-01: ReadingRecord constructor");
}

// TC-R-02: 読了日「不明」設定
bool testReadingRecordSetDateUnknown()
{
    ReadingRecord record;
    const bool result = record.setDate("不明");
    return check(result && record.getDate() == "不明", "TC-R-02: ReadingRecord setDate unknown");
}

// TC-R-03: 読了日(6桁)正常設定
bool testReadingRecordSetDateValid()
{
    ReadingRecord record;
    const bool result = record.setDate("260801");
    return check(result && record.getDate() == "260801", "TC-R-03: ReadingRecord setDate valid (260801)");
}

// TC-R-04: 読了日不正文字数拒否
bool testReadingRecordSetDateInvalid()
{
    ReadingRecord record;
    const bool result = record.setDate("26080");
    return check(!result, "TC-R-04: ReadingRecord setDate invalid (5 digits)");
}

// TC-R-05: 読了日空文字自動補完（本日日付6桁）
bool testReadingRecordSetDateEmptyAuto()
{
    ReadingRecord record;
    const bool result = record.setDate("");
    const std::string d = record.getDate();
    // 6桁数字であり、月・日の範囲が正常か確認
    const bool validTodayFormat = (d.length() == 6) &&
        std::all_of(d.begin(), d.end(), ::isdigit) &&
        (std::stoi(d.substr(2, 2)) >= 1 && std::stoi(d.substr(2, 2)) <= 12) &&
        (std::stoi(d.substr(4, 2)) >= 1 && std::stoi(d.substr(4, 2)) <= 31);
    return check(result && validTodayFormat, "TC-R-05: ReadingRecord setDate empty auto-complete");
}

// TC-R-06: タイトル正常設定
bool testReadingRecordSetTitleValid()
{
    ReadingRecord record;
    const bool result = record.setTitle("人間失格");
    return check(result && record.getTitle() == "人間失格", "TC-R-06: ReadingRecord setTitle valid");
}

// TC-R-07: タイトル空文字自動補完
bool testReadingRecordSetTitleEmpty()
{
    ReadingRecord record;
    const bool result = record.setTitle("");
    return check(result && record.getTitle() == "Unknown_title", "TC-R-07: ReadingRecord setTitle empty auto-complete");
}

// TC-R-08: タイトル境界値 50文字設定
bool testReadingRecordSetTitle50Characters()
{
    ReadingRecord record;
    const std::string title(50, 'A');
    const bool result = record.setTitle(title);
    return check(result && record.getTitle() == title, "TC-R-08: ReadingRecord setTitle 50 characters");
}

// TC-R-09: タイトル境界値 51文字拒否
bool testReadingRecordSetTitle51Characters()
{
    ReadingRecord record;
    const std::string title(51, 'A');
    const bool result = record.setTitle(title);
    return check(!result, "TC-R-09: ReadingRecord setTitle 51 characters rejected");
}

// TC-R-10: 著者名正常設定
bool testReadingRecordSetAuthorValid()
{
    ReadingRecord record;
    const bool result = record.setAuthor("太宰治");
    return check(result && record.getAuthor() == "太宰治", "TC-R-10: ReadingRecord setAuthor valid");
}

// TC-R-11: 著者名空文字自動補完
bool testReadingRecordSetAuthorEmpty()
{
    ReadingRecord record;
    const bool result = record.setAuthor("");
    return check(result && record.getAuthor() == "Unknown_author", "TC-R-11: ReadingRecord setAuthor empty auto-complete");
}

// TC-R-12: 著者名境界値 50文字設定
bool testReadingRecordSetAuthor50Characters()
{
    ReadingRecord record;
    const std::string author(50, 'A');
    const bool result = record.setAuthor(author);
    return check(result && record.getAuthor() == author, "TC-R-12: ReadingRecord setAuthor 50 characters");
}

// TC-R-13: 著者名境界値 51文字拒否
bool testReadingRecordSetAuthor51Characters()
{
    ReadingRecord record;
    const std::string author(51, 'A');
    const bool result = record.setAuthor(author);
    return check(!result, "TC-R-13: ReadingRecord setAuthor 51 characters rejected");
}

// TC-R-14: ジャンル設定
bool testReadingRecordSetGenre()
{
    ReadingRecord record;
    record.setGenre(0, "小説・物語");
    return check(record.getGenreIndex() == 0 && record.getGenreName() == "小説・物語", "TC-R-14: ReadingRecord setGenre");
}

// TC-R-15: 全回答設定
bool testReadingRecordSetAnswers()
{
    ReadingRecord record;
    const std::vector<std::string> answers{ "回答1", "回答2", "回答3", "(スキップ)", "回答5", "回答6" };
    record.setAnswers(answers);
    return check(record.getAnswers() == answers, "TC-R-15: ReadingRecord setAnswers");
}

// TC-R-16: 個別回答変更
bool testReadingRecordSetAnswer()
{
    ReadingRecord record;
    const std::vector<std::string> answers{ "回答1", "回答2", "回答3", "回答4", "回答5", "回答6" };
    record.setAnswers(answers);
    record.setAnswer(1, "修正後の回答");
    const bool unchanged = record.getAnswer(0) == "回答1" &&
        record.getAnswer(2) == "回答3" &&
        record.getAnswer(3) == "回答4" &&
        record.getAnswer(4) == "回答5" &&
        record.getAnswer(5) == "回答6";
    return check(record.getAnswer(1) == "修正後の回答" && unchanged, "TC-R-16: ReadingRecord setAnswer");
}

int runQuestionTests()
{
    int failed = 0;
    std::cout << "\n--- QuestionSet tests ---\n";
    failed += !testQuestionSetConstructor();
    failed += !testQuestionSetGetGenreName();
    failed += !testQuestionSetGetGenreDescription();
    failed += !testQuestionSetGetQuestions();
    failed += !testQuestionSetGetQuestionFirst();
    failed += !testQuestionSetGetQuestionLast();
    failed += !testQuestionSetGetQuestionCount();

    std::cout << "\n--- ReadingRecord tests ---\n";
    failed += !testReadingRecordConstructor();
    failed += !testReadingRecordSetDateUnknown();
    failed += !testReadingRecordSetDateValid();
    failed += !testReadingRecordSetDateInvalid();
    failed += !testReadingRecordSetDateEmptyAuto();
    failed += !testReadingRecordSetTitleValid();
    failed += !testReadingRecordSetTitleEmpty();
    failed += !testReadingRecordSetTitle50Characters();
    failed += !testReadingRecordSetTitle51Characters();
    failed += !testReadingRecordSetAuthorValid();
    failed += !testReadingRecordSetAuthorEmpty();
    failed += !testReadingRecordSetAuthor50Characters();
    failed += !testReadingRecordSetAuthor51Characters();
    failed += !testReadingRecordSetGenre();
    failed += !testReadingRecordSetAnswers();
    failed += !testReadingRecordSetAnswer();

    return failed;
}