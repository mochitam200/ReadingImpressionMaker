#include "../../include/Question.hpp" // テスト対象のQuestionSetとReadingRecordを使うために読み込みます。
#include <iostream> // テスト結果を画面へ表示するために使います。
#include <string> // 文字列を扱うために使います。
#include <vector> // 質問と回答の配列を扱うために使います.

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

bool testQuestionSetConstructor() // QuestionSetのコンストラクタをテストします。
{ // テスト開始です。
    const std::vector<std::string> questions{"質問1", "質問2", "質問3", "質問4"}; // テスト用に4つの質問を作ります。
    const QuestionSet questionSet("小説・物語", questions); // ジャンル名と質問を使ってQuestionSetを作ります。
    return check(questionSet.getGenreName() == "小説・物語" && questionSet.getQuestions() == questions, "QuestionSet constructor"); // 作成した内容が正しいか確認します。
} // テスト終了です。

bool testQuestionSetGetGenreName() // ジャンル名のgetterをテストします。
{ // テスト開始です。
    const QuestionSet questionSet("実用書", {"質問1", "質問2", "質問3", "質問4"}); // テスト用のQuestionSetを作ります。
    return check(questionSet.getGenreName() == "実用書", "QuestionSet getGenreName"); // ジャンル名が正しく取得できるか確認します。
} // テスト終了です。

bool testQuestionSetGetQuestions() // 質問一覧のgetterをテストします。
{ // テスト開始です。
    const std::vector<std::string> questions{"質問1", "質問2", "質問3", "質問4"}; // 期待する質問一覧を作ります。
    const QuestionSet questionSet("実用書", questions); // テスト用のQuestionSetを作ります。
    return check(questionSet.getQuestions() == questions, "QuestionSet getQuestions"); // 質問一覧が正しく取得できるか確認します。
} // テスト終了です。

bool testQuestionSetGetQuestionFirst() // 最初の質問を取得できるかテストします。
{ // テスト開始です。
    const QuestionSet questionSet("小説・物語", {"質問1", "質問2", "質問3", "質問4"}); // 4問のテストデータを作ります。
    return check(questionSet.getQuestion(0) == "質問1", "QuestionSet getQuestion first"); // 0番目の質問が1問目か確認します。
} // テスト終了です。

bool testQuestionSetGetQuestionLast() // 最後の質問を取得できるかテストします。
{ // テスト開始です。
    const QuestionSet questionSet("小説・物語", {"質問1", "質問2", "質問3", "質問4"}); // 4問のテストデータを作ります。
    return check(questionSet.getQuestion(3) == "質問4", "QuestionSet getQuestion last"); // 3番目の質問が4問目か確認します。
} // テスト終了です。

bool testQuestionSetGetQuestionCount() // 質問数を取得できるかテストします。
{ // テスト開始です。
    const QuestionSet questionSet("小説・物語", {"質問1", "質問2", "質問3", "質問4"}); // 4問のテストデータを作ります。
    return check(questionSet.getQuestionCount() == 4, "QuestionSet getQuestionCount"); // 質問数が4になっているか確認します。
} // テスト終了です。

bool testReadingRecordConstructor() // ReadingRecordを生成できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // ReadingRecordを生成します。
    (void)record; // 生成した変数を使っていないという警告を防ぎます。
    return check(true, "ReadingRecord constructor"); // 仕様書で初期値が定義されていないため生成できたことを確認します。
} // テスト終了です。

bool testReadingRecordSetDateValid() // 正しい読了日を設定できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const bool result = record.setDate("260801"); // 正常な6桁の日付を設定します。
    return check(result && record.getDate() == "260801", "ReadingRecord setDate valid"); // setterの戻り値と保存された値を確認します。
} // テスト終了です。

bool testReadingRecordSetDateInvalid() // 不正な読了日を拒否できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const bool result = record.setDate("26080"); // 5桁の不正な日付を設定してみます。
    return check(!result, "ReadingRecord setDate invalid"); // 不正な入力なのでfalseになることを確認します。
} // テスト終了です。

bool testReadingRecordSetTitleValid() // 正常なタイトルを設定できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const bool result = record.setTitle("人間失格"); // 正常なタイトルを設定します。
    return check(result && record.getTitle() == "人間失格", "ReadingRecord setTitle valid"); // 設定成功と保存内容を確認します。
} // テスト終了です。

bool testReadingRecordSetTitleEmpty() // 空のタイトルを拒否できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const bool result = record.setTitle(""); // 空文字を設定してみます。
    return check(!result, "ReadingRecord setTitle empty"); // 空文字なのでfalseになることを確認します。
} // テスト終了です。

bool testReadingRecordSetTitle50Characters() // 50文字のタイトルを受け付けるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const std::string title(50, 'A'); // ASCII文字を50個並べて50文字のタイトルを作ります。
    const bool result = record.setTitle(title); // 50文字のタイトルを設定します。
    return check(result && record.getTitle() == title, "ReadingRecord setTitle 50 characters"); // 上限値が受理されるか確認します。
} // テスト終了です。

bool testReadingRecordSetTitle51Characters() // 51文字のタイトルを拒否できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const std::string title(51, 'A'); // ASCII文字を51個並べて上限超過のタイトルを作ります。
    const bool result = record.setTitle(title); // 51文字のタイトルを設定してみます。
    return check(!result, "ReadingRecord setTitle 51 characters"); // 上限を超えているためfalseになることを確認します。
} // テスト終了です。

bool testReadingRecordSetAuthorValid() // 正常な著者名を設定できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const bool result = record.setAuthor("太宰治"); // 著者名を設定します。
    return check(result && record.getAuthor() == "太宰治", "ReadingRecord setAuthor valid"); // 設定成功と保存内容を確認します。
} // テスト終了です。

bool testReadingRecordSetAuthorEmpty() // 空の著者名を拒否できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const bool result = record.setAuthor(""); // 空文字を設定してみます。
    return check(!result, "ReadingRecord setAuthor empty"); // 空文字なのでfalseになることを確認します。
} // テスト終了です。

bool testReadingRecordSetGenre() // ジャンルを設定できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    record.setGenre(0, "小説・物語"); // 0番目のジャンルとして小説・物語を設定します。
    return check(record.getGenreIndex() == 0 && record.getGenreName() == "小説・物語", "ReadingRecord setGenre"); // 番号と名前の両方を確認します。
} // テスト終了です。

bool testReadingRecordSetAnswers() // 4つの回答をまとめて設定できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const std::vector<std::string> answers{"回答1", "回答2", "(スキップ)", "回答4"}; // テスト用の4つの回答を作ります。
    record.setAnswers(answers); // 回答一覧を設定します。
    return check(record.getAnswers() == answers, "ReadingRecord setAnswers"); // 設定した回答一覧と取得結果を比較します。
} // テスト終了です。

bool testReadingRecordSetAnswer() // 1つだけ回答を修正できるかテストします。
{ // テスト開始です。
    ReadingRecord record; // テスト対象のReadingRecordを作ります。
    const std::vector<std::string> answers{"回答1", "回答2", "回答3", "回答4"}; // 4つの初期回答を作ります。
    record.setAnswers(answers); // 初期回答を設定します。
    record.setAnswer(1, "修正後の回答"); // 2問目だけを修正します。
    const bool unchanged = record.getAnswer(0) == "回答1" && record.getAnswer(2) == "回答3" && record.getAnswer(3) == "回答4"; // 他の3問が変わっていないことを確認します。
    return check(record.getAnswer(1) == "修正後の回答" && unchanged, "ReadingRecord setAnswer"); // 修正対象だけが変わったことを確認します。
} // テスト終了です。

int runQuestionTests() // QuestionSetとReadingRecordの全テストを実行します。
{ // 関数開始です。
    int failed = 0; // 失敗件数を0で開始します。
    std::cout << "\n--- QuestionSet tests ---\n"; // QuestionSetテストの見出しを表示します。
    failed += !testQuestionSetConstructor(); // コンストラクタのテスト結果を集計します。
    failed += !testQuestionSetGetGenreName(); // ジャンル名getterのテスト結果を集計します。
    failed += !testQuestionSetGetQuestions(); // 質問一覧getterのテスト結果を集計します。
    failed += !testQuestionSetGetQuestionFirst(); // 先頭質問のテスト結果を集計します。
    failed += !testQuestionSetGetQuestionLast(); // 最後の質問のテスト結果を集計します。
    failed += !testQuestionSetGetQuestionCount(); // 質問数のテスト結果を集計します。
    std::cout << "\n--- ReadingRecord tests ---\n"; // ReadingRecordテストの見出しを表示します。
    failed += !testReadingRecordConstructor(); // コンストラクタのテスト結果を集計します。
    failed += !testReadingRecordSetDateValid(); // 正常な日付のテスト結果を集計します。
    failed += !testReadingRecordSetDateInvalid(); // 不正な日付のテスト結果を集計します。
    failed += !testReadingRecordSetTitleValid(); // 正常なタイトルのテスト結果を集計します。
    failed += !testReadingRecordSetTitleEmpty(); // 空タイトルのテスト結果を集計します。
    failed += !testReadingRecordSetTitle50Characters(); // 50文字タイトルのテスト結果を集計します。
    failed += !testReadingRecordSetTitle51Characters(); // 51文字タイトルのテスト結果を集計します。
    failed += !testReadingRecordSetAuthorValid(); // 正常な著者名のテスト結果を集計します。
    failed += !testReadingRecordSetAuthorEmpty(); // 空著者名のテスト結果を集計します。
    failed += !testReadingRecordSetGenre(); // ジャンル設定のテスト結果を集計します。
    failed += !testReadingRecordSetAnswers(); // 回答一覧のテスト結果を集計します。
    failed += !testReadingRecordSetAnswer(); // 個別回答修正のテスト結果を集計します。
    return failed; // このファイルで発生した失敗件数を返します。
} // 関数終了です。
