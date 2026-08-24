#include "../../include/ConsoleUI.hpp" // テスト対象のConsoleUIを使うために読み込みます。
#include <iostream> // std::cinとテスト結果表示に使います。
#include <sstream> // テスト用の入力ストリームを作るために使います。
#include <string> // 文字列を扱うために使います。
#include <vector> // QuestionSetの一覧を作るために使います。

class CinRedirect // std::cinを一時的にテスト用文字列へ差し替えるクラスです。
{ // クラスの開始です。
private: // 以下をクラス内部だけで使えるようにします。
    std::streambuf* originalBuffer_; // 差し替える前のstd::cinの入力バッファを保存します。
    std::istringstream inputStream_; // テストで入力する文字列をストリームとして保持します。

public: // 以下を外部から利用できるようにします。
    explicit CinRedirect(const std::string& input) : originalBuffer_(std::cin.rdbuf()), inputStream_(input) // テスト入力を設定し、元の入力先を保存します。
    { // コンストラクタの開始です。
        std::cin.rdbuf(inputStream_.rdbuf()); // std::cinをテスト用ストリームへ変更します。
    } // コンストラクタの終了です。

    ~CinRedirect() // テスト終了時に呼ばれるデストラクタです。
    { // デストラクタの開始です。
        std::cin.rdbuf(originalBuffer_); // std::cinを元の入力先へ戻します。
    } // デストラクタの終了です。
};

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



std::vector<QuestionSet> createTestQuestionSets() // 5ジャンル分のテストデータを作ります。
{
    return {
        QuestionSet("小説・物語", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
        QuestionSet("実用書", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
        QuestionSet("ビジネス・自己啓発", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
        QuestionSet("専門書・学習書", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"}),
        QuestionSet("ノンフィクション・伝記", {"質問1", "質問2", "質問3", "質問4", "質問5", "質問6"})
    }; // 初期化したvectorを返します。
} // 関数終了です。

std::string makeString50() // ASCII文字50文字のテストデータを作ります。
{ // 関数開始です。
    return std::string(50, 'A'); // Aを50個並べて返します。
} // 関数終了です。

std::string makeString51() // ASCII文字51文字のテストデータを作ります。
{ // 関数開始です。
    return std::string(51, 'A'); // Aを51個並べて返します。
} // 関数終了です。

} // 無名名前空間の終了です。

bool testInputDateUnknown() // 読了日「不明」をテストします。
{
    CinRedirect input("不明\n");
    const std::string result = ConsoleUI::inputDate();
    return check(result == "不明", "inputDate 不明");
}

bool testInputDateValidMin() // YYMMDD形式の最小値260101をテストします。
{ // テスト開始です。
    CinRedirect input("260101\n"); // 260101を入力した状態を作ります。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "260101", "inputDate 260101"); // 入力値がそのまま受理されたか確認します。
} // テスト終了です。

bool testInputDateValidMax() // YYMMDD形式の最大値261231をテストします。
{ // テスト開始です。
    CinRedirect input("261231\n"); // 261231を入力した状態を作ります。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "261231", "inputDate 261231"); // 入力値が受理されたか確認します。
} // テスト終了です。

bool testInputDateInvalidMonthAndDay() // 月や日の範囲外を入力した後に再入力できるかテストします。
{ // テスト開始です。
    CinRedirect input("260001\n261301\n260832\n260801\n"); // 不正な3種類を入力した後、正しい値を入力します。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "260801", "inputDate invalid month/day then valid"); // 最後の正しい値が受理されたか確認します。
} // テスト終了です。

bool testInputDateValidMonth() // YYMM形式の最小値をテストします。
{ // テスト開始です。
    CinRedirect input("2601\n"); // 2601を入力します。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "2601", "inputDate 2601"); // 正常に受理されたか確認します。
} // テスト終了です。

bool testInputDateValidMonthMax() // YYMM形式の最大値をテストします。
{ // テスト開始です。
    CinRedirect input("2612\n"); // 2612を入力します。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "2612", "inputDate 2612"); // 正常に受理されたか確認します。
} // テスト終了です。

bool testInputDateInvalidMonthFormat() // YYMM形式の範囲外を拒否できるかテストします。
{ // テスト開始です。
    CinRedirect input("2600\n2613\n2601\n"); // 不正な2つを入力してから正常値を入力します。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "2601", "inputDate invalid YYMM then valid"); // 正常値が受理されたか確認します。
} // テスト終了です。

bool testInputDateSeasons() // 4種類の季節表記をそれぞれテストします。
{ // テスト開始です。
    bool ok = true; // 全テストが成功したかを保存します。
    { CinRedirect input("26春\n"); ok = ok && ConsoleUI::inputDate() == "26春"; } // 春の入力が受理されるか確認します。
    { CinRedirect input("26夏\n"); ok = ok && ConsoleUI::inputDate() == "26夏"; } // 夏の入力が受理されるか確認します。
    { CinRedirect input("26秋\n"); ok = ok && ConsoleUI::inputDate() == "26秋"; } // 秋の入力が受理されるか確認します。
    { CinRedirect input("26冬\n"); ok = ok && ConsoleUI::inputDate() == "26冬"; } // 冬の入力が受理されるか確認します。
    return check(ok, "inputDate four seasons"); // 4つすべて成功したか確認します。
} // テスト終了です。

bool testInputDateInvalidSeason() // 不正な季節表記を拒否できるかテストします。
{ // テスト開始です。
    CinRedirect input("26初夏\n26Spring\n26春\n"); // 不正な2つを入力してから正常な季節を入力します。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "26春", "inputDate invalid season then valid"); // 正常な季節だけが受理されたか確認します。
} // テスト終了です。

bool testInputDateEmpty() // 空入力を拒否して再入力できるかテストします。
{ // テスト開始です。
    CinRedirect input("\n260801\n"); // 空入力の後に正常値を入力します。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "260801", "inputDate empty then valid"); // 正常値が受理されたか確認します。
} // テスト終了です。

bool testInputDateDifferentFormat() // 指定されていない日付形式を拒否するかテストします。
{ // テスト開始です。
    CinRedirect input("2026/08/01\n260801\n"); // 不正形式の後に正常値を入力します。
    const std::string result = ConsoleUI::inputDate(); // 読了日入力処理を実行します。
    return check(result == "260801", "inputDate different format then valid"); // 正常値が受理されたか確認します。
} // テスト終了です。

bool testInputTitleValid() // 通常のタイトルを入力できるかテストします。
{ // テスト開始です。
    CinRedirect input("人間失格\n"); // タイトルを入力します。
    const std::string result = ConsoleUI::inputTitle(); // タイトル入力処理を実行します。
    return check(result == "人間失格", "inputTitle normal"); // 入力されたタイトルが返るか確認します。
} // テスト終了です。

bool testInputTitleQ84() // qを含む1Q84を中断操作と誤認しないかテストします。
{ // テスト開始です。
    CinRedirect input("1Q84\n"); // 1Q84を入力します。
    const std::string result = ConsoleUI::inputTitle(); // タイトル入力処理を実行します。
    return check(result == "1Q84", "inputTitle 1Q84"); // 1Q84がそのままタイトルになるか確認します。
} // テスト終了です。

bool testInputTitle50Characters() // タイトル50文字を受理するかテストします。
{ // テスト開始です。
    const std::string title = makeString50(); // 50文字のタイトルを作ります。
    CinRedirect input(title + "\n"); // 50文字タイトルを入力します。
    const std::string result = ConsoleUI::inputTitle(); // タイトル入力処理を実行します。
    return check(result == title, "inputTitle 50 characters"); // 上限値が受理されたか確認します。
} // テスト終了です。

bool testInputTitle51Characters() // タイトル51文字を拒否して再入力できるかテストします。
{ // テスト開始です。
    const std::string title51 = makeString51(); // 51文字のタイトルを作ります。
    CinRedirect input(title51 + "\n人間失格\n"); // 51文字を入力した後に正常値を入力します。
    const std::string result = ConsoleUI::inputTitle(); // タイトル入力処理を実行します。
    return check(result == "人間失格", "inputTitle 51 characters then valid"); // 正常値が最終的に受理されたか確認します。
} // テスト終了です。

bool testInputTitleEmpty() // 空入力を拒否して再入力できるかテストします。
{ // テスト開始です。
    CinRedirect input("\n人間失格\n"); // 空入力の後に正常値を入力します。
    const std::string result = ConsoleUI::inputTitle(); // タイトル入力処理を実行します。
    return check(result == "人間失格", "inputTitle empty then valid"); // 正常値が受理されたか確認します。
} // テスト終了です。

bool testInputTitleSpaces() // スペースだけのタイトルを拒否できるかテストします。
{ // テスト開始です。
    CinRedirect input("   \n人間失格\n"); // スペースだけを入力した後に正常値を入力します。
    const std::string result = ConsoleUI::inputTitle(); // タイトル入力処理を実行します。
    return check(result == "人間失格", "inputTitle spaces then valid"); // 正常値が受理されたか確認します。
} // テスト終了です。

bool testInputAuthorValid() // 通常の著者名を入力できるかテストします。
{ // テスト開始です。
    CinRedirect input("太宰治\n"); // 著者名を入力します。
    const std::string result = ConsoleUI::inputAuthor(); // 著者入力処理を実行します。
    return check(result == "太宰治", "inputAuthor normal"); // 入力された著者名が返るか確認します。
} // テスト終了です。

bool testInputAuthorEmpty() // 空の著者名を拒否できるかテストします。
{ // テスト開始です。
    CinRedirect input("\n太宰治\n"); // 空入力の後に正常値を入力します。
    const std::string result = ConsoleUI::inputAuthor(); // 著者入力処理を実行します。
    return check(result == "太宰治", "inputAuthor empty then valid"); // 正常値が受理されたか確認します。
} // テスト終了です。

bool testInputAuthor50Characters() // 著者名50文字を受理するかテストします。
{ // テスト開始です。
    const std::string author = makeString50(); // 50文字の著者名を作ります。
    CinRedirect input(author + "\n"); // 50文字の著者名を入力します。
    const std::string result = ConsoleUI::inputAuthor(); // 著者入力処理を実行します。
    return check(result == author, "inputAuthor 50 characters"); // 上限値が受理されたか確認します。
} // テスト終了です。

bool testSelectGenreFirst() // ジャンル1を選択して確定できるかテストします。
{ // テスト開始です。
    const auto questions = createTestQuestionSets(); // 5ジャンルのテストデータを作ります。
    CinRedirect input("1\ny\n"); // 1番と確定のyを入力します。
    const int result = ConsoleUI::selectGenre(questions); // ジャンル選択処理を実行します。
    return check(result == 0, "selectGenre 1"); // 0-indexedの0が返るか確認します。
} // テスト終了です。

bool testSelectGenreLast() // ジャンル5を選択して確定できるかテストします。
{ // テスト開始です。
    const auto questions = createTestQuestionSets(); // 5ジャンルのテストデータを作ります。
    CinRedirect input("5\ny\n"); // 5番と確定のyを入力します。
    const int result = ConsoleUI::selectGenre(questions); // ジャンル選択処理を実行します。
    return check(result == 4, "selectGenre 5"); // 0-indexedの4が返るか確認します。
} // テスト終了です。

bool testSelectGenreInvalidThenValid() // 範囲外入力後に正常値を入力できるかテストします。
{ // テスト開始です。
    const auto questions = createTestQuestionSets(); // 5ジャンルのテストデータを作ります。
    CinRedirect input("0\n6\na\n\n1\ny\n"); // 0、6、a、空入力の後に1とyを入力します。
    const int result = ConsoleUI::selectGenre(questions); // ジャンル選択処理を実行します。
    return check(result == 0, "selectGenre invalid then valid"); // 最終的に1番目が選択されたか確認します。
} // テスト終了です。

bool testSelectGenreCancelThenSelect() // nでジャンル選択を取り消して再選択できるかテストします。
{ // テスト開始です。
    const auto questions = createTestQuestionSets(); // 5ジャンルのテストデータを作ります。
    CinRedirect input("1\nn\n2\ny\n"); // 1番を選んでnで取り消し、その後2番をyで確定します。
    const int result = ConsoleUI::selectGenre(questions); // ジャンル選択処理を実行します。
    return check(result == 1, "selectGenre cancel then select 2"); // 2番目が選択されたか確認します。
} // テスト終了です。

bool testAskQuestionsOneLine() // 1行回答をテストします。
{
    const QuestionSet questionSet("小説・物語", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("回答1\n\n回答2\n\n回答3\n\n回答4\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    return check(answers.size() == 6 && answers[0] == "回答1" && answers[5] == "回答6", "askQuestions one line");
}

bool testAskQuestionsSkip() // skipを入力するとスキップになるかテストします。
{
    const QuestionSet questionSet("小説・物語", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("skip\nskip\nskip\nskip\nskip\nskip\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    return check(answers.size() == 6 && answers[0] == "(スキップ)" && answers[5] == "(スキップ)", "askQuestions all skip");
}

bool testAskQuestionsSkipWordIsNotPartialMatch() // skipperなどをskipと誤認しないかテストします。
{
    const QuestionSet questionSet("小説・物語", { "質問1", "質問2", "質問3", "質問4", "質問5", "質問6" });
    CinRedirect input("skipper\n\nskipです\n\n通常回答\n\nskip\n\n回答5\n\n回答6\n\n");
    const auto answers = ConsoleUI::askQuestions(questionSet);
    const bool first = answers.size() >= 1 && answers[0] == "skipper";
    const bool second = answers.size() >= 2 && answers[1] == "skipです";
    const bool fourth = answers.size() >= 4 && answers[3] == "(スキップ)";
    return check(first && second && fourth, "askQuestions skip partial match");
} // テスト終了です。

int runConsoleUITests() // ConsoleUIの全テストを実行します。
{ // 関数開始です。
    int failed = 0; // 失敗件数を0で開始します。
    std::cout << "\n--- ConsoleUI tests ---\n"; // ConsoleUIテストの見出しを表示します。
    failed += !testInputDateValidMin(); // 260101の結果を集計します。
    failed += !testInputDateValidMax(); // 261231の結果を集計します。
    failed += !testInputDateInvalidMonthAndDay(); // 月日範囲外の結果を集計します。
    failed += !testInputDateValidMonth(); // 2601の結果を集計します。
    failed += !testInputDateValidMonthMax(); // 2612の結果を集計します。
    failed += !testInputDateInvalidMonthFormat(); // YYMM範囲外の結果を集計します。
    failed += !testInputDateSeasons(); // 4季節の結果を集計します。
    failed += !testInputDateInvalidSeason(); // 不正季節の結果を集計します。
    failed += !testInputDateEmpty(); // 空入力の結果を集計します。
    failed += !testInputDateDifferentFormat(); // 別形式の結果を集計します。
    failed += !testInputTitleValid(); // 通常タイトルの結果を集計します。
    failed += !testInputTitleQ84(); // 1Q84の結果を集計します。
    failed += !testInputTitle50Characters(); // 50文字タイトルの結果を集計します。
    failed += !testInputTitle51Characters(); // 51文字タイトルの結果を集計します。
    failed += !testInputTitleEmpty(); // 空タイトルの結果を集計します。
    failed += !testInputTitleSpaces(); // スペースタイトルの結果を集計します。
    failed += !testInputAuthorValid(); // 通常著者名の結果を集計します。
    failed += !testInputAuthorEmpty(); // 空著者名の結果を集計します。
    failed += !testInputAuthor50Characters(); // 50文字著者名の結果を集計します。
    failed += !testSelectGenreFirst(); // ジャンル1の結果を集計します。
    failed += !testSelectGenreLast(); // ジャンル5の結果を集計します。
    failed += !testSelectGenreInvalidThenValid(); // 不正入力後の正常入力の結果を集計します。
    failed += !testSelectGenreCancelThenSelect(); // nで取り消した後の再選択結果を集計します。
    failed += !testAskQuestionsOneLine(); // 1行回答の結果を集計します。
    failed += !testAskQuestionsSkip(); // 全問skipの結果を集計します。
    failed += !testAskQuestionsSkipWordIsNotPartialMatch(); // skipperなどの結果を集計します。
    return failed; // このファイルの失敗件数を返します。
} // 関数終了です。
