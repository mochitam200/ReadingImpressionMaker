#pragma once
#ifndef RECORD_EXPORTER_HPP
#define RECORD_EXPORTER_HPP

#include "Question.hpp" // ReadingRecordやQuestionSetを使うために読み込み
#include <filesystem>   // フォルダの作成や存在確認を行うためのライブラリ
#include <fstream>      // ファイル書き込み（std::ofstream）を行うために読み込み
#include <string>       // 文字列を扱うために読み込み
#include <vector>       // 配列を扱うために読み込み
#include <clocale>   // ロケール設定のため

namespace fs = std::filesystem; // std::filesystem を fs という短い名前で使えるようする

// ファイルへの書き出し処理を行うクラス
class RecordExporter {
private:
    // OSのファイル名で使用できない禁止文字（\ / : * ? " < > |）を '_'（アンダースコア）に置換する関数
    static std::string sanitizeFilename(const std::string& name) {
        std::string result = name;
        const std::string forbidden = "\\/:*?\"<>|"; // Windows等でファイル名に使えない禁止文字の一覧を登録
        for (char& c : result) {   // result の中にある文字を先頭から1文字ずつ取り出して調べる
            if (forbidden.find(c) != std::string::npos) { // 禁止文字が見つかった場合
                c = '_'; // 安全なアンダースコアに置き換え
            }
        }
        return result;  // 安全なファイル名になった result を返す
    }

public:
    // 読書記録と質問セットを受け取り、指定されたフォーマットでテキストファイルを出力
    static bool exportToFile(const ReadingRecord& record, const std::vector<QuestionSet>& questions) {
        try {

            std::setlocale(LC_ALL, ".UTF-8"); // UTF-8パスおよびファイル名の文字コード変換エラーを防止

            // 保存先となる "output" フォルダが存在しない場合、自動的に作成
            if (!fs::exists("output")) {
                fs::create_directories("output");
            }

            // ファイル名で問題が起きないように日付・タイトル・著者名をサニタイズ処理
            std::string dateStr = sanitizeFilename(record.getDate());
            std::string titleStr = sanitizeFilename(record.getTitle());
            std::string authorStr = sanitizeFilename(record.getAuthor());

            // 出力先のファイルパス（例: output/260801_人間失格_太宰治.txt）を組み立て
            fs::path filePath = fs::path("output") / (dateStr + "_" + titleStr + "_" + authorStr + ".txt");

            // BOMを正確に出力できるようバイナリモードでファイルを開く
            std::ofstream file(filePath, std::ios::binary);
            if (!file.is_open()) return false; // ファイルの作成に失敗した場合は false を返す

            // 文字化けを防ぐため、ファイルの先頭に UTF-8 BOM（3バイト: 0xEF, 0xBB, 0xBF）を書き込む
            const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };   // bom という配列に、UTF-8のBOMを表す3バイトを入れる
            file.write(reinterpret_cast<const char*>(bom), sizeof(bom));  // bom に入っている 0xEF 0xBB 0xBF の3バイトをファイルへ書き込む

            // ヘッダー情報（読了日、タイトル、著者、ジャンル）を出力
            file << "読了日: " << record.getDate() << "\n";
            file << "タイトル: " << record.getTitle() << "\n";
            file << "著者: " << record.getAuthor() << "\n";
            file << "ジャンル: " << record.getGenreName() << "\n\n";

            // 記録に設定されているジャンルに対応する質問セットを探す
            const QuestionSet* targetQs = nullptr;
            if (record.getGenreIndex() >= 0 && static_cast<size_t>(record.getGenreIndex()) < questions.size()) {
                targetQs = &questions[record.getGenreIndex()];
            }
            else { // インデックスが見つからない場合はジャンル名で検索を試みる
                for (const auto& qs : questions) {
                    if (qs.getGenreName() == record.getGenreName()) {
                        targetQs = &qs;
                        break;
                    }
                }
            }

            // 各質問と対応する回答をペアにして出力
            const auto& answers = record.getAnswers();
            if (targetQs) {
                for (size_t i = 0; i < targetQs->getQuestionCount(); ++i) {
                    file << "質問" << (i + 1) << ": " << targetQs->getQuestion(i) << "\n";
                    if (i < answers.size()) {
                        file << "回答: " << answers[i] << "\n\n";
                    }
                }
            }

            file.close(); // 書き込みを確定してファイルを閉じる
            return true;  // 出力成功を呼び出し元へ通知
        }
        catch (...) {
            return false; // エラーや例外が発生した場合は失敗(false)を返す
        }
    }
};

#endif