#include "../include/Question.hpp"       // QuestionSet, ReadingRecord クラスを使用
#include "../include/CsvLoader.hpp"      // CSV読み込み機能を使用
#include "../include/ConsoleUI.hpp"      // コンソール入力機能を使用
#include "../include/RecordExporter.hpp" // ファイル出力機能を使用
#include <iostream>                   // コンソール出力に使用
#include <string>                     // 文字列を扱うため
#include <vector>                     // 配列を扱うため
#include <clocale>                    // ロケール設定のため

int main()
{
    // 日本語の入出力を正しく扱うためにロケールを設定
    std::setlocale(LC_ALL, ".UTF-8");

    std::cout << "========================================\n";
    std::cout << " 読書感想メーカー (ReadingImpressionMaker)\n";
    std::cout << "========================================\n\n";

    // ── 1. CSVファイルから質問一覧を読み込み ──
    const std::string csvFilename = "book_review_questions.csv";
    std::vector<QuestionSet> questionSets = CsvLoader::load(csvFilename);

    if (questionSets.empty()) { // CSVの読み込みに失敗した場合
        std::cerr << "エラー: 質問ファイル「" << csvFilename << "」の読み込みに失敗しました。\n";
        std::cerr << "ファイルが存在し、正しい形式（4列×30行以上）であることを確認してください。\n";
        return 1; // 異常終了
    }

    std::cout << "質問ファイルを読み込みました（" << questionSets.size() << " ジャンル）\n\n";

    // ── 2. 読了日の入力 ──
    std::cout << "--- 読了日を入力してください ---\n";
    std::cout << "  形式: YYMMDD（例: 260801）/ YYMM（例: 2608）/ YY+季節（例: 26夏）/ 不明\n";
    std::cout << "> ";
    std::string date = ConsoleUI::inputDate();

    if (date.empty()) { // 入力が取得できなかった場合
        std::cerr << "エラー: 読了日の入力に失敗しました。\n";
        return 1;
    }

    // ReadingRecord に読了日を設定
    ReadingRecord record;
    record.setDate(date);
    std::cout << "読了日: " << record.getDate() << "\n\n";

    // ── 3. タイトルの入力 ──
    std::cout << "--- 書籍タイトルを入力してください（1〜50文字） ---\n";
    std::cout << "> ";
    std::string title = ConsoleUI::inputTitle();

    if (title.empty()) {
        std::cerr << "エラー: タイトルの入力に失敗しました。\n";
        return 1;
    }

    record.setTitle(title);
    std::cout << "タイトル: " << record.getTitle() << "\n\n";

    // ── 4. 著者名の入力 ──
    std::cout << "--- 著者名を入力してください（1〜50文字） ---\n";
    std::cout << "> ";
    std::string author = ConsoleUI::inputAuthor();

    if (author.empty()) {
        std::cerr << "エラー: 著者名の入力に失敗しました。\n";
        return 1;
    }

    record.setAuthor(author);
    std::cout << "著者: " << record.getAuthor() << "\n\n";

    // ── 5. ジャンル選択 ──
    std::cout << "--- ジャンルを選択してください ---\n";
    for (size_t i = 0; i < questionSets.size(); ++i) { // ジャンル一覧を表示
        std::cout << "  " << (i + 1) << ". " << questionSets[i].getGenreName() << "\n";
    }
    std::cout << "番号を入力 > ";
    int genreIndex = ConsoleUI::selectGenre(questionSets);

    if (genreIndex < 0) { // ジャンル選択に失敗した場合
        std::cerr << "エラー: ジャンルの選択に失敗しました。\n";
        return 1;
    }

    record.setGenre(genreIndex, questionSets[genreIndex].getGenreName());
    std::cout << "ジャンル: " << record.getGenreName() << "\n\n";

    // ── 6. 質問への回答入力 ──
    std::cout << "--- 質問に回答してください ---\n";
    std::cout << "  （回答を入力後、空行で次の質問へ進みます。\"skip\" でスキップ）\n\n";

    const QuestionSet& selectedQs = questionSets[genreIndex];
    for (size_t i = 0; i < selectedQs.getQuestionCount(); ++i) { // 質問文を事前に表示
        std::cout << "  Q" << (i + 1) << ": " << selectedQs.getQuestion(i) << "\n";
    }
    std::cout << "\n";

    std::vector<std::string> answers = ConsoleUI::askQuestions(selectedQs);
    record.setAnswers(answers);

    std::cout << "\n--- 入力内容の確認 ---\n";
    std::cout << "読了日:     " << record.getDate() << "\n";
    std::cout << "タイトル:   " << record.getTitle() << "\n";
    std::cout << "著者:       " << record.getAuthor() << "\n";
    std::cout << "ジャンル:   " << record.getGenreName() << "\n";
    for (size_t i = 0; i < answers.size(); ++i) {
        std::cout << "回答" << (i + 1) << ":     " << answers[i] << "\n";
    }
    std::cout << "\n";

    // ── 7. ファイル出力 ──
    std::cout << "ファイルへ出力しています...\n";
    bool exportResult = RecordExporter::exportToFile(record, questionSets);

    if (exportResult) {
        std::cout << "出力が完了しました！（outputフォルダを確認してください）\n";
    }
    else {
        std::cerr << "エラー: ファイルの出力に失敗しました。\n";
        return 1;
    }

    std::cout << "\n========================================\n";
    std::cout << " ご利用ありがとうございました\n";
    std::cout << "========================================\n";

    return 0; // 正常終了
}