#include "../include/ConsoleUI.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>

// 日付を入力する関数（正しい形式になるまで繰り返す）
std::string ConsoleUI::inputDate() {
    std::string line;
    // ユーザーが文字を入力してEnterを押すたびにループ処理をする
    while (std::getline(std::cin, line)) {
        // 何も入力せずにEnterを押した場合は「今日の日付」を自動で設定して返す
        if (line.empty()) {
            return ReadingRecord::getTodayDate();
        }
        // 日付の形式が正しいかチェックし、OKならそのまま返す
        if (ReadingRecord::isValidDate(line)) {
            return line;
        }
    }
    return ""; // 入力エラー（ファイル末尾到達など）が発生した場合は空文字を返す
}

// タイトルを入力する関数
std::string ConsoleUI::inputTitle() {
    std::string line;
    while (std::getline(std::cin, line)) {
        // 未入力（そのままEnter）なら「Unknown_title」を返す
        if (line.empty()) {
            return "Unknown_title";
        }
        // スペースやタブなどの空白文字だけが入力されているかチェック
        bool allWhitespace = std::all_of(line.begin(), line.end(), [](unsigned char c) {
            return std::isspace(c);
            });
        if (allWhitespace) {
            return "Unknown_title";
        }
        // 文字列の長さが規定範囲内であれば入力採用
        if (ReadingRecord::isValidStringLength(line)) {
            return line;
        }
    }
    return "";
}

// 著者名を入力する関数
std::string ConsoleUI::inputAuthor() {
    std::string line;
    while (std::getline(std::cin, line)) {
        // 未入力なら「Unknown_author」を返す
        if (line.empty()) {
            return "Unknown_author";
        }
        // 空白文字だけの入力かをチェック
        bool allWhitespace = std::all_of(line.begin(), line.end(), [](unsigned char c) {
            return std::isspace(c);
            });
        if (allWhitespace) {
            return "Unknown_author";
        }
        // 文字列の長さがOKであれば入力採用
        if (ReadingRecord::isValidStringLength(line)) {
            return line;
        }
    }
    return "";
}

// ジャンル（番号）を選択する関数
int ConsoleUI::selectGenre(const std::vector<QuestionSet>& questions) {
    std::string line;
    while (std::getline(std::cin, line)) {
        // 入力が空でなく、すべて数字であるかを確認
        if (!line.empty() && std::all_of(line.begin(), line.end(), ::isdigit)) {
            // 文字列を数値（int型）に変換
            int choice = std::stoi(line);
            // 入力された番号が選択肢の範囲内（1 〜 質問セットの数）か確認
            if (choice >= 1 && choice <= static_cast<int>(questions.size())) {
                // 配列のインデックス（0始まり）に合わせて -1 した値を返す
                return choice - 1;
            }
        }
        // 範囲外の数値や文字を入力された場合は警告を出して再入力
        std::cout << "無効な入力です。1 〜 " << questions.size() << " の番号を入力してください。\n> ";
    }
    return -1; // エラー時は -1 を返す
}

// 複数の質問に対する回答を順番に入力・収集する関数
std::vector<std::string> ConsoleUI::askQuestions(const QuestionSet& questionSet) {
    std::vector<std::string> answers; // 回答を入れておく配列（リスト）
    size_t count = questionSet.getQuestionCount(); // 質問の総数を取得

    // 質問の数だけ順番に処理を繰り返す
    for (size_t i = 0; i < count; ++i) {
        // 画面に質問を表示する
        std::cout << "\n----------------------------------------\n";
        std::cout << " Q" << (i + 1) << ": " << questionSet.getQuestion(i) << "\n";
        std::cout << "----------------------------------------\n> ";

        std::string line;
        std::string fullAnswer; // 複数行入力されたテキストをまとめる変数
        bool isFirstLine = true;
        bool skipped = false;

        // ユーザーの入力を1行ずつ受け取る（Enterを2回押すと入力終了）
        while (std::getline(std::cin, line)) {
            // 最初の行で "skip" と入力されたら質問を飛ばす
            if (isFirstLine && line == "skip") {
                skipped = true;
                break; // ループを抜ける
            }

            // 空白行（そのままEnterを押した）なら入力完了とみなしてループを抜ける
            if (line.empty()) {
                break;
            }

            // 行末に '\'（バックスラッシュ）がある場合は消去して改行入力として扱う
            if (!line.empty() && line.back() == '\\') {
                line.pop_back();
            }

            // 2行目以降なら改行コードを追加してから連結する
            if (!isFirstLine) {
                fullAnswer += "\n";
            }
            fullAnswer += line;
            isFirstLine = false;
        }

        // スキップされたか、何も書かれなかった場合は「(スキップ)」として記録
        if (skipped || fullAnswer.empty()) {
            answers.push_back("(スキップ)");
        }
        else {
            answers.push_back(fullAnswer);
        }
    }

    return answers; // 揃ったすべての回答を返す
}