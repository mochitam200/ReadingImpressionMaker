#pragma once
#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "Question.hpp" // ReadingRecordやQuestionSetのバリデーション機能を使うため
#include <iostream>     // コンソール入力（std::cin）を扱うため
#include <string>       // 文字列を扱うため
#include <vector>       // 配列を扱うため
#include <algorithm>    // 数字判定など
#include <cctype>       // std::isdigit等

// 画面入力の処理をまとめたクラス
class ConsoleUI {
public:
    // 正しい読了日（YYMMDD/YYMM/YY+季節/不明）が入力されるまで繰り返し入力を受け取り（未入力時は本日日付）
    static std::string inputDate() {
        std::string line;
        while (std::getline(std::cin, line)) { // ユーザーの入力を1行読み込み
            if (line.empty()) {
                return ReadingRecord::getTodayDate(); // 未入力（Enterのみ）時は本日日付を自動設定
            }
            if (ReadingRecord::isValidDate(line)) { // 正しい形式か検証
                return line; // 正しければ入力された文字列を返す
            }
            // 不正な入力だった場合はループが継続し、次の入力を待つ
        }
        return "";
    }

    // 正しいタイトル（1〜50文字、全空白でない）が入力されるまで繰り返し読み込み（未入力・空白のみ時はUnknown_title）
    static std::string inputTitle() {
        std::string line;
        while (std::getline(std::cin, line)) { // ユーザーの入力を1行読み込み
            if (line.empty()) {
                return "Unknown_title"; // 未入力（Enterのみ）時は Unknown_title
            }
            bool allWhitespace = std::all_of(line.begin(), line.end(), [](unsigned char c) {
                return std::isspace(c);
                });
            if (allWhitespace) {
                return "Unknown_title"; // 空白のみ時は Unknown_title
            }
            if (ReadingRecord::isValidStringLength(line)) { // 入力された line が正しいタイトルかチェック
                return line; // タイトルが正しい場合、その入力された文字列を関数の呼び出し元に返して終了
            }
        }
        return ""; // 最後まで正しいタイトルを受け取れなかった場合は空文字列を返す
    }

    // 正しい著者名（1〜50文字、全空白でない）が入力されるまで繰り返し読み込み（未入力・空白のみ時はUnknown_author）
    static std::string inputAuthor() {
        std::string line;
        while (std::getline(std::cin, line)) { // ユーザーの入力を1行読み込み
            if (line.empty()) {
                return "Unknown_author"; // 未入力（Enterのみ）時は Unknown_author
            }
            bool allWhitespace = std::all_of(line.begin(), line.end(), [](unsigned char c) {
                return std::isspace(c);
                });
            if (allWhitespace) {
                return "Unknown_author"; // 空白のみ時は Unknown_author
            }
            if (ReadingRecord::isValidStringLength(line)) { // 入力された line が正しい著者名かチェック
                return line; // 著者名が正しい場合、その入力された文字列を関数の呼び出し元に返して終了
            }
        }
        return ""; // 最後まで正しい著者名を受け取れなかった場合は空文字列を返す
    }

    // ジャンル選択番号を受け取ってインデックス番号（0始まり）を返す
    static int selectGenre(const std::vector<QuestionSet>& questions) {
        std::string line;
        while (std::getline(std::cin, line)) { // 番号入力を受け取る
            // 入力が数値のみで構成されているか確認
            if (!line.empty() && std::all_of(line.begin(), line.end(), ::isdigit)) {
                int choice = std::stoi(line);   // 入力された文字列を数値に変換
                // 選択番号が 1 〜 ジャンル数 の範囲内にあるかチェック
                if (choice >= 1 && choice <= static_cast<int>(questions.size())) {
                    return choice - 1; // プログラム内で扱いやすいよう「0始まりの番号」で返す
                }
            }
            // 範囲外の数字や無効な入力だった場合の案内表示（任意）
            std::cout << "無効な入力です。1 〜 " << questions.size() << " の番号を入力してください。\n> ";
        }
        return -1; // 入力が終了してしまった場合はエラー値を返す
    }

    // 質問ごとにユーザーからの回答入力を収集する関数（startNumberで開始質問番号を指定可能）
    static std::vector<std::string> askQuestions(const QuestionSet& questionSet, size_t startNumber = 1) {
        std::vector<std::string> answers; // 各質問への回答を格納する配列
        size_t count = questionSet.getQuestionCount(); // 質問の総数を取得

        for (size_t i = 0; i < count; ++i) {
            std::cout << "\n----------------------------------------\n";
            std::cout << " Q" << (startNumber + i) << ": " << questionSet.getQuestion(i) << "\n";
            std::cout << "----------------------------------------\n> ";

            std::string line;
            std::string fullAnswer; // 複数行にわたる回答を結合するための文字列
            bool isFirstLine = true; // 1行目の入力かどうかを識別するフラグ
            bool skipped = false;

            while (std::getline(std::cin, line)) {
                // 最初の1行目がちょうど "skip" だった場合、スキップ処理とみなす
                if (isFirstLine && line == "skip") {
                    skipped = true;
                    break; // この質問への回答入力を終了して次の質問へ進む
                }

                // 空行（何も入力せずにEnter）が押されたら、その質問の回答終了と判定
                if (line.empty()) {
                    break;
                }

                // 改行エスケープ（末尾が '\'）の場合は末尾の '\' を取り除く
                if (!line.empty() && line.back() == '\\') {
                    line.pop_back();
                }

                // 2行目以降の入力がある場合は、改行文字で結合
                if (!isFirstLine) {
                    fullAnswer += "\n";
                }
                fullAnswer += line;
                isFirstLine = false; // 1行目の処理が終わったのでフラグを下げる
            }

            if (skipped || fullAnswer.empty()) {
                answers.push_back("(スキップ)"); // スキップ用の文字列を設定
            }
            else {
                answers.push_back(fullAnswer); // 組み立てた回答を配列に追加
            }
        }

        return answers; // すべての質問への回答リストを返す
    }
};

#endif