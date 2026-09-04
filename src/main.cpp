#include "../include/Question.hpp"       // QuestionSet, ReadingRecord クラスを使用
#include "../include/CsvLoader.hpp"      // CSV読み込み機能を使用
#include "../include/ConsoleUI.hpp"      // コンソール入力機能を使用
#include "../include/RecordExporter.hpp" // ファイル出力機能を使用
#include <iostream>                   // コンソール出力に使用
#include <string>                     // 文字列を扱うため
#include <vector>                     // 配列を扱うため
#include <sstream>                    // 文字列ストリーム
#include <clocale>                    // ロケール設定のため
#include <algorithm>                  // all_of 等

// 補助関数: 文字列の各行にインデントを付加して表示
static void printFormattedAnswer(const std::string& answer) {
    std::stringstream ss(answer);
    std::string line;
    bool isFirst = true;
    while (std::getline(ss, line)) {
        if (isFirst) {
            std::cout << "    → " << line << "\n";
            isFirst = false;
        } else {
            std::cout << "      " << line << "\n";
        }
    }
    if (isFirst) {
        std::cout << "    → \n";
    }
}

// 補助関数: ジャンル一覧の表示
static void displayGenreList(const std::vector<QuestionSet>& questionSets) {
    static const char* circledDigits[] = {
        "①", "②", "③", "④", "⑤", "⑥", "⑦", "⑧", "⑨", "⑩",
        "⑪", "⑫", "⑬", "⑭", "⑮", "⑯", "⑰", "⑱", "⑲", "⑳"
    };

    std::cout << "【OK！次は本のジャンルを教えてください】\n\n";
    for (size_t i = 0; i < questionSets.size(); ++i) {
        std::string numStr;
        if (i < 20) {
            numStr = circledDigits[i];
        } else {
            numStr = "(" + std::to_string(i + 1) + ")";
        }
        std::cout << numStr << " " << questionSets[i].getGenreName();
        if (!questionSets[i].getGenreDescription().empty()) {
            std::cout << " (" << questionSets[i].getGenreDescription() << ")";
        }
        std::cout << "\n";
    }
    std::cout << "\n番号を入力してください (1〜" << questionSets.size() << ")\n> ";
}

// 補助関数: 最終確認画面の表示
static void displayConfirmation(const ReadingRecord& record, const QuestionSet& selectedQs) {
    std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "  ✅ 入力内容の確認\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
    std::cout << "[1] 読了日     : " << record.getDate() << "\n";
    std::cout << "[2] タイトル   : " << record.getTitle() << "\n";
    std::cout << "[3] 著者       : " << record.getAuthor() << "\n";
    std::cout << "[4] ジャンル   : " << record.getGenreName() << "\n\n";
    std::cout << "--- 質問と回答 ---\n\n";

    const auto& answers = record.getAnswers();
    for (size_t i = 0; i < selectedQs.getQuestionCount(); ++i) {
        std::cout << "[" << (i + 5) << "] 【質問" << (i + 1) << "】" << selectedQs.getQuestion(i) << "\n";
        if (i < answers.size()) {
            printFormattedAnswer(answers[i]);
        } else {
            std::cout << "    → (スキップ)\n";
        }
        std::cout << "\n";
    }
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
}

int main()
{
    // 日本語の入出力を正しく扱うためにロケールを設定
    std::setlocale(LC_ALL, ".UTF-8");

    // ── 1. CSVファイルから質問一覧を読み込み ──
    const std::string csvFilename = "book_review_questions.csv";
    std::vector<QuestionSet> questionSets = CsvLoader::load(csvFilename);

    if (questionSets.empty()) { // CSVの読み込みに失敗した場合
        std::cerr << "[エラー] " << csvFilename << " が見つかりません。\n";
        std::cerr << "プログラムと同じフォルダに " << csvFilename << " を配置してください。\n";
        return 1; // 異常終了
    }

    // ── 2. アプリケーションタイトルと読了日の入力 ──
    std::cout << "┌─────────────────────────────────────────────┐\n";
    std::cout << "│  📖 読書感想記録メーカー                      │\n";
    std::cout << "└─────────────────────────────────────────────┘\n\n";

    std::cout << "【いつ頃読了しましたか？以下のどれかで入力してください。】\n";
    std::cout << "・6桁の数字(例：260101)\n";
    std::cout << "・何年+何月(例：2601)\n";
    std::cout << "・何年+季節(例：26春)\n";
    std::cout << "・不明\n";
    std::cout << "※空欄のままEnterを押すと本日の日付6桁が適用されます\n";
    std::cout << "> ";

    std::string date = ConsoleUI::inputDate();
    if (date.empty()) { // 入力が取得できなかった場合
        std::cerr << "エラー: 読了日の入力に失敗しました。\n";
        return 1;
    }

    ReadingRecord record;
    record.setDate(date);

    // ── 3. 書籍情報（タイトル・著者名）の入力 ──
    std::cout << "\n【OK！まずは書籍の情報を教えてください】\n";
    std::cout << "※空欄でEnterを押した場合、タイトルは `Unknown_title`、著者は `Unknown_author` を自動適用します\n\n";

    std::cout << "本のタイトルは？(50文字以内)\n";
    std::cout << "> ";
    std::string title = ConsoleUI::inputTitle();
    if (title.empty()) {
        std::cerr << "エラー: タイトルの入力に失敗しました。\n";
        return 1;
    }
    record.setTitle(title);

    std::cout << "\n著者は？(50文字以内)\n";
    std::cout << "> ";
    std::string author = ConsoleUI::inputAuthor();
    if (author.empty()) {
        std::cerr << "エラー: 著者名の入力に失敗しました。\n";
        return 1;
    }
    record.setAuthor(author);
    std::cout << "\n";

    // ── 4. ジャンル選択 ──
    displayGenreList(questionSets);
    int genreIndex = ConsoleUI::selectGenre(questionSets);
    if (genreIndex < 0 || genreIndex >= static_cast<int>(questionSets.size())) {
        std::cerr << "エラー: ジャンルの選択に失敗しました。\n";
        return 1;
    }
    record.setGenre(genreIndex, questionSets[genreIndex].getGenreName());

    // ── 5. ジャンル別質問への回答入力 ──
    std::cout << "\n--- 質問に回答してください ---\n";
    std::cout << "  ※ 改行したい場合は文末に \\ を入力してEnterで次の行へ\n";
    std::cout << "  ※ 回答を終了するには空行でEnterを押してください\n";
    std::cout << "  ※ この質問をスキップするには「skip」と入力してください(未入力でEnter入力でもスキップされます)\n\n";

    for (size_t i = 0; i < questionSets[genreIndex].getQuestionCount(); ++i) {
        std::cout << "  Q" << (i + 1) << ": " << questionSets[genreIndex].getQuestion(i) << "\n";
    }
    std::cout << "\n> ";

    std::vector<std::string> answers = ConsoleUI::askQuestions(questionSets[genreIndex]);
    record.setAnswers(answers);

    // ── 6. 最終確認および個別修正ループ ──
    while (true) {
        displayConfirmation(record, questionSets[genreIndex]);

        std::cout << "修正したい項目の番号を入力してください（修正なしなら Enter で保存）\n";
        std::cout << "> ";

        std::string choiceStr;
        if (!std::getline(std::cin, choiceStr)) {
            break; // EOF
        }

        // Enter（空文字）で修正完了・ファイル保存へ
        if (choiceStr.empty()) {
            break;
        }

        // 'q' または 'Q' で保存せず終了
        if (choiceStr == "q" || choiceStr == "Q") {
            std::cout << "保存せずに終了しますか？ (y/n)\n> ";
            std::string confirm;
            if (std::getline(std::cin, confirm) && (confirm == "y" || confirm == "Y")) {
                std::cout << "保存せずに終了します。\n";
                return 0;
            }
            continue;
        }

        // 数値判定
        if (!std::all_of(choiceStr.begin(), choiceStr.end(), ::isdigit)) {
            std::cout << "無効な入力です。番号を入力してください。\n";
            continue;
        }

        int choice = std::stoi(choiceStr);
        int totalQuestions = static_cast<int>(questionSets[genreIndex].getQuestionCount());

        if (choice == 1) { // 読了日の修正
            std::cout << "\n【いつ頃読了しましたか？以下のどれかで入力してください。】\n";
            std::cout << "・6桁の数字(例：260101)\n";
            std::cout << "・何年+何月(例：2601)\n";
            std::cout << "・何年+季節(例：26春)\n";
            std::cout << "・不明\n";
            std::cout << "※空欄のままEnterを押すと本日の日付6桁が適用されます\n";
            std::cout << "> ";
            std::string newDate = ConsoleUI::inputDate();
            if (!newDate.empty()) {
                record.setDate(newDate);
            }
        }
        else if (choice == 2) { // タイトルの修正
            std::cout << "\n本のタイトルは？(50文字以内)\n";
            std::cout << "> ";
            std::string newTitle = ConsoleUI::inputTitle();
            if (!newTitle.empty()) {
                record.setTitle(newTitle);
            }
        }
        else if (choice == 3) { // 著者名の修正
            std::cout << "\n著者は？(50文字以内)\n";
            std::cout << "> ";
            std::string newAuthor = ConsoleUI::inputAuthor();
            if (!newAuthor.empty()) {
                record.setAuthor(newAuthor);
            }
        }
        else if (choice == 4) { // ジャンルの修正
            std::cout << "ジャンルを変更すると回答がリセットされます。よろしいですか？(y/n)\n> ";
            std::string conf;
            if (std::getline(std::cin, conf) && (conf == "y" || conf == "Y")) {
                std::cout << "\n";
                displayGenreList(questionSets);
                int newGenreIndex = ConsoleUI::selectGenre(questionSets);
                if (newGenreIndex >= 0 && newGenreIndex < static_cast<int>(questionSets.size())) {
                    genreIndex = newGenreIndex;
                    record.setGenre(genreIndex, questionSets[genreIndex].getGenreName());

                    std::cout << "\n--- 質問に回答してください ---\n";
                    std::cout << "  ※ 改行したい場合は文末に \\ を入力してEnterで次の行へ\n";
                    std::cout << "  ※ 回答を終了するには空行でEnterを押してください\n";
                    std::cout << "  ※ この質問をスキップするには「skip」と入力してください(未入力でEnter入力でもスキップされます)\n\n";

                    for (size_t i = 0; i < questionSets[genreIndex].getQuestionCount(); ++i) {
                        std::cout << "  Q" << (i + 1) << ": " << questionSets[genreIndex].getQuestion(i) << "\n";
                    }
                    std::cout << "\n> ";

                    std::vector<std::string> newAnswers = ConsoleUI::askQuestions(questionSets[genreIndex]);
                    record.setAnswers(newAnswers);
                }
            }
        }
        else if (choice >= 5 && choice <= 4 + totalQuestions) { // 各質問の回答修正
            size_t qIdx = static_cast<size_t>(choice - 5);
            std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
            std::cout << "  📝 質問 " << (qIdx + 1) << "/" << totalQuestions << " (修正)\n";
            std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
            std::cout << questionSets[genreIndex].getQuestion(qIdx) << "\n\n";
            std::cout << "※ 改行したい場合は文末に \\ を入力してEnterで次の行へ\n";
            std::cout << "※ 回答を終了するには空行でEnterを押してください\n";
            std::cout << "※ この質問をスキップするには「skip」と入力してください(未入力でEnter入力でもスキップされます)\n\n";
            std::cout << "> ";

            QuestionSet singleQs(
                questionSets[genreIndex].getGenreName(),
                questionSets[genreIndex].getGenreDescription(),
                { questionSets[genreIndex].getQuestion(qIdx) }
            );
            std::vector<std::string> singleAns = ConsoleUI::askQuestions(singleQs);
            if (!singleAns.empty()) {
                record.setAnswer(qIdx, singleAns[0]);
            }
        }
        else {
            std::cout << "指定された項目番号は存在しません。\n";
        }
    }

    // ── 7. ファイル出力 ──
    bool exportResult = RecordExporter::exportToFile(record, questionSets);

    if (exportResult) {
        std::cout << "\n📁 ファイルを保存しました！\n";
        std::cout << "   output/" << record.getDate() << "_" << record.getTitle() << "_" << record.getAuthor() << ".txt\n\n";
        std::cout << "ご利用ありがとうございました！📚\n";
    }
    else {
        std::cerr << "エラー: ファイルの出力に失敗しました。\n";
        return 1;
    }

    return 0; // 正常終了
}