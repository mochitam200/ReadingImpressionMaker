#include "../../include/CsvLoader.hpp" // テスト対象のCsvLoaderを使うために読み込みます。
#include <cstdio> // テスト用CSVファイルを削除するstd::removeを使います。
#include <fstream> // テスト用CSVファイルを作成するために使います。
#include <iostream> // テスト結果を表示するために使います。
#include <string> // ファイル名などの文字列を扱うために使います。

namespace // このファイルだけで使う補助関数を名前空間で囲みます。
{ // 無名名前空間の開始です。

    bool check(bool condition, const std::string& name) // テスト結果を共通表示する関数です。
    { // check関数の開始です。
        if (condition) // 期待した結果になったか確認します。
        { // 成功時の処理を開始します。
            std::cout << "[PASS] " << name << '\n'; // 成功したテスト名を表示します。
            return true; // 成功を返します。
        } // 成功時の処理を終了します。
        std::cout << "[FAIL] " << name << '\n'; // 失敗したテスト名を表示します。
        return false; // 失敗を返します。
    } // check関数の終了です。

    void writeTextFile(const std::string& filename, const std::string& text) // 指定した文字列をテストファイルへ書き込む関数です。
    { // 関数開始です。
        std::ofstream file(filename, std::ios::binary); // バイナリモードでテストファイルを開きます。
        file << text; // 引数で受け取ったテキストを書き込みます。
    } // 関数終了です。

    std::string makeValidCsv(const std::string& newline = "\n") // 仕様に合う30行のCSV文字列を作ります。
    { // 関数開始です。
        std::string csv; // 完成したCSVを入れる文字列を作ります。
        csv += "1,小説・物語,1,一言で紹介するならどんな本ですか" + newline;
        csv += "1,小説・物語,2,一番印象に残った登場人物は誰？（好き・嫌い・気になった理由も）" + newline;
        csv += "1,小説・物語,3,一番息をのんだ「名シーン」や「名台詞」はどこ？" + newline;
        csv += "1,小説・物語,4,この本を読んでいる時、心に一番残った感情は？" + newline;
        csv += "1,小説・物語,5,もし自分が物語の中に入れたら、どの場面で主人公にどんな声をかけたい？" + newline;
        csv += "1,小説・物語,6,最後に何かあればご自由にお書きください" + newline;
        csv += "2,実用書,1,一言で紹介するならどんな本ですか" + newline;
        csv += "2,実用書,2,「これはすぐ使える！」と膝を打ったテクニックやノウハウは？" + newline;
        csv += "2,実用書,3,読んだ今日・明日から、具体的に試してみたい「小さな一歩（ToDo）」は？" + newline;
        csv += "2,実用書,4,逆に「これは今の自分には合わない・不要だ」と思った部分は？" + newline;
        csv += "2,実用書,5,この本の要点を、まだ読んでいない友達に一言で教えるなら？" + newline;
        csv += "2,実用書,6,最後に何かあればご自由にお書きください" + newline;
        csv += "3,ビジネス・自己啓発,1,一言で紹介するならどんな本ですか" + newline;
        csv += "3,ビジネス・自己啓発,2,ページをめくる手が止まった「ハイライトしたい一言（フレーズ）」は？" + newline;
        csv += "3,ビジネス・自己啓発,3,今までの自分の「常識」や「言い訳」を打ち砕かれた部分はどこ？" + newline;
        csv += "3,ビジネス・自己啓発,4,この本を読む前と読んだ後で、自分の「行動」や「選択」はどう変わりそう？" + newline;
        csv += "3,ビジネス・自己啓発,5,1ヶ月後の自分が忘れないように、スマホの待受けにしたいレベルの「教訓」は？" + newline;
        csv += "3,ビジネス・自己啓発,6,最後に何かあればご自由にお書きください" + newline;
        csv += "4,専門書・学習書,1,一言で紹介するならどんな本ですか" + newline;
        csv += "4,専門書・学習書,2,この本を読む前と後で、一番「解像度が上がった」概念やテーマは？" + newline;
        csv += "4,専門書・学習書,3,専門用語や理論の中で、「なるほど、そういうことか！」とスッキリした部分は？" + newline;
        csv += "4,専門書・学習書,4,読んでいて「ここをもっと調べたい」「まだ謎が残る」と思った課題は？" + newline;
        csv += "4,専門書・学習書,5,この知識を手に入れたことで、次に読んでみたい本や挑戦したいステップは？" + newline;
        csv += "4,専門書・学習書,6,最後に何かあればご自由にお書きください" + newline;
        csv += "5,ノンフィクション・伝記,1,一言で紹介するならどんな本ですか" + newline;
        csv += "5,ノンフィクション・伝記,2,主人公（または登場人物）の「生き方」や「決断」で、一番震えたシーンは？" + newline;
        csv += "5,ノンフィクション・伝記,3,自分がもし同じ時代・同じ立場にいたら、同じ決断ができたと思う？" + newline;
        csv += "5,ノンフィクション・伝記,4,この本を読む前と後で、ニュースや世の中に対する「見え方」はどう変わった？" + newline;
        csv += "5,ノンフィクション・伝記,5,著者の情熱や生き方から、自分の人生に「感染」させたいエネルギーや姿勢は？" + newline;
        csv += "5,ノンフィクション・伝記,6,最後に何かあればご自由にお書きください" + newline;
        return csv;
    } // 関数終了です。

} // 無名名前空間の終了です。

bool testCsvValidLF() // LF改行の正常なCSVをテストします。
{ // テスト開始です。
    const std::string filename = "test_valid_lf.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, makeValidCsv("\n")); // LF改行の30行CSVを作ります。
    const auto result = CsvLoader::load(filename); // CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.size() == 5 && result[0].getQuestionCount() == 6 && result[4].getQuestionCount() == 6, "CsvLoader valid CSV LF"); // 5ジャンル×6問として読み込めたか確認します。
} // テスト終了です。

bool testCsvValidCRLF() // CRLF改行の正常なCSVをテストします。
{ // テスト開始です。
    const std::string filename = "test_valid_crlf.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, makeValidCsv("\r\n")); // Windowsで一般的なCRLF改行のCSVを作ります。
    const auto result = CsvLoader::load(filename); // CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.size() == 5 && result[0].getQuestionCount() == 6, "CsvLoader valid CSV CRLF"); // CRLFでも正常に読み込めるか確認します。
} // テスト終了です。

bool testCsvValidBom() // UTF-8 BOM付きCSVをテストします。
{ // テスト開始です。
    const std::string filename = "test_valid_bom.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, std::string("\xEF\xBB\xBF") + makeValidCsv()); // UTF-8 BOMを先頭につけたCSVを作ります。
    const auto result = CsvLoader::load(filename); // BOM付きCSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.size() == 5 && result[0].getGenreName() == "小説・物語", "CsvLoader UTF-8 BOM"); // BOMがあってもジャンル名を正しく取得できるか確認します。
} // テスト終了です。

bool testCsvFileNotFound() // CSVファイルが存在しない場合をテストします。
{ // テスト開始です。
    std::remove("test_not_found.csv"); // 同名のテストファイルが残っていた場合に先に削除します。
    const auto result = CsvLoader::load("test_not_found.csv"); // 存在しないCSVを読み込みます。
    return check(result.empty(), "CsvLoader file not found"); // 失敗時に空vectorが返る仕様を確認します。
} // テスト終了です。

bool testCsvEmpty() // 0バイトのCSVをテストします。
{ // テスト開始です。
    const std::string filename = "test_empty.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, ""); // 中身が空のファイルを作ります。
    const auto result = CsvLoader::load(filename); // 空CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.empty(), "CsvLoader empty CSV"); // 不正なCSVとして空vectorが返ることを確認します。
} // テスト終了です。

bool testCsvLessThan30Rows() // 30行未満（29行）のCSVをテストします。
{ // テスト開始です。
    const std::string filename = "test_29_rows.csv"; // テスト用ファイル名を決めます。
    std::string csv; // 29行分のCSVを入れる文字列を作ります。
    for (int i = 0; i < 29; ++i) // 29回繰り返して29行作ります。
    { // 繰り返しの開始です。
        csv += "1,小説・物語," + std::to_string(i + 1) + ",質問\n"; // 1行のCSVデータを追加します。
    } // 繰り返しの終了です。
    writeTextFile(filename, csv); // 29行のCSVをファイルへ書き込みます。
    const auto result = CsvLoader::load(filename); // CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.empty(), "CsvLoader less than 30 rows"); // 30行未満ならエラーとして扱う仕様を確認します。
} // テスト終了です。

bool testCsvTooFewColumns() // カラム数が3つしかない行をテストします。
{ // テスト開始です。
    const std::string filename = "test_few_columns.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, "1,小説・物語,1\n"); // 4列必要なのに3列しかない行を書き込みます。
    const auto result = CsvLoader::load(filename); // CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.empty(), "CsvLoader too few columns"); // 不正なカラム数として失敗することを確認します。
} // テスト終了です。

bool testCsvTooManyColumns() // カラム数が5つある行をテストします。
{ // テスト開始です。
    const std::string filename = "test_many_columns.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, "1,小説・物語,1,質問文,余分\n"); // 4列必要なのに5列ある行を書き込みます。
    const auto result = CsvLoader::load(filename); // CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.empty(), "CsvLoader too many columns"); // 不正なカラム数として失敗することを確認します。
} // テスト終了です。

bool testCsvInvalidGenreNumber() // ジャンル番号が文字の場合をテストします。
{ // テスト開始です。
    const std::string filename = "test_invalid_genre.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, "A,小説・物語,1,質問文\n"); // ジャンル番号にAを入れた不正データを作ります。
    const auto result = CsvLoader::load(filename); // CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.empty(), "CsvLoader invalid genre number"); // 数値でないジャンル番号を拒否できるか確認します。
} // テスト終了です。

bool testCsvInvalidQuestionNumber() // 質問番号が文字の場合をテストします。
{ // テスト開始です。
    const std::string filename = "test_invalid_question.csv"; // テスト用ファイル名を決めます。
    writeTextFile(filename, "1,小説・物語,A,質問文\n"); // 質問番号にAを入れた不正データを作ります。
    const auto result = CsvLoader::load(filename); // CSVを読み込みます。
    std::remove(filename.c_str()); // テスト終了後にファイルを削除します。
    return check(result.empty(), "CsvLoader invalid question number"); // 数値でない質問番号を拒否できるか確認します。
} // テスト終了です。

int runCsvLoaderTests() // CsvLoaderの全テストを実行します。
{ // 関数開始です。
    int failed = 0; // 失敗件数を0で開始します。
    std::cout << "\n--- CsvLoader tests ---\n"; // テストの見出しを表示します。
    failed += !testCsvValidLF(); // LF正常系の結果を集計します。
    failed += !testCsvValidCRLF(); // CRLF正常系の結果を集計します。
    failed += !testCsvValidBom(); // BOM付き正常系の結果を集計します。
    failed += !testCsvFileNotFound(); // ファイル不在の結果を集計します。
    failed += !testCsvEmpty(); // 空CSVの結果を集計します。
    failed += !testCsvLessThan30Rows(); // 29行CSVの結果を集計します。
    failed += !testCsvTooFewColumns(); // カラム不足の結果を集計します。
    failed += !testCsvTooManyColumns(); // カラム過多の結果を集計します。
    failed += !testCsvInvalidGenreNumber(); // ジャンル番号不正の結果を集計します。
    failed += !testCsvInvalidQuestionNumber(); // 質問番号不正の結果を集計します。
    return failed; // このファイルの失敗件数を返します。
} // 関数終了です。