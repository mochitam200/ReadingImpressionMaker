#include "../include/RecordExporter.hpp"
#include <filesystem>
#include <fstream>
#include <clocale>

namespace fs = std::filesystem;

// --------------------------------------------------
// ファイル名に使えない文字を「_」に置き換える関数
// --------------------------------------------------
std::string RecordExporter::sanitizeFilename(const std::string& name) {
    std::string result = name;

    // Windowsなどでファイル名に使えない禁忌文字のリスト
    const std::string forbidden = "\\/:*?\"<>|";

    // 文字列を1文字ずつチェックし、使えない文字があれば「_」に変える
    for (char& c : result) {
        if (forbidden.find(c) != std::string::npos) {
            c = '_';
        }
    }
    return result;
}

// --------------------------------------------------
// 読書記録をテキストファイルに出力する関数
// --------------------------------------------------
bool RecordExporter::exportToFile(const ReadingRecord& record, const std::vector<QuestionSet>& questions) {
    try {
        // 日本語などの文字化けを防ぐため、文字コードをUTF-8に設定
        std::setlocale(LC_ALL, ".UTF-8");

        // 「output」フォルダが存在しない場合は自動で作成する
        if (!fs::exists("output")) {
            fs::create_directories("output");
        }

        // ファイル名に使うため、日付・タイトル・著者名から使えない文字を除去
        std::string dateStr = sanitizeFilename(record.getDate());
        std::string titleStr = sanitizeFilename(record.getTitle());
        std::string authorStr = sanitizeFilename(record.getAuthor());

        // 保存先となるファイルパスを作成（例: output/2026-03-05_タイトル_著者.txt）
        fs::path filePath = fs::path("output") / (dateStr + "_" + titleStr + "_" + authorStr + ".txt");

        // ファイルをバイナリモードで開く
        std::ofstream file(filePath, std::ios::binary);
        if (!file.is_open()) return false; // 開けない場合は失敗として終了

        // UTF-8形式であることを示す目印（BOM）をファイルの先頭に書き込む
        const unsigned char bom[] = { 0xEF, 0xBB, 0xBF };
        file.write(reinterpret_cast<const char*>(bom), sizeof(bom));

        // 基本情報（日付、タイトル、著者、ジャンル）をファイルに書き込む
        file << "読了日: " << record.getDate() << "\n";
        file << "タイトル: " << record.getTitle() << "\n";
        file << "著者: " << record.getAuthor() << "\n";
        file << "ジャンル: " << record.getGenreName() << "\n\n";

        // この本に対応する質問リストを探す
        const QuestionSet* targetQs = nullptr;

        // 1. まずはインデックス番号を使って探す
        if (record.getGenreIndex() >= 0 && static_cast<size_t>(record.getGenreIndex()) < questions.size()) {
            targetQs = &questions[record.getGenreIndex()];
        }
        // 2. インデックスで見つからない場合は、ジャンル名で一致するものを探す
        else {
            for (const auto& qs : questions) {
                if (qs.getGenreName() == record.getGenreName()) {
                    targetQs = &qs;
                    break;
                }
            }
        }

        // 質問とそれに対する回答を順番にファイルに書き込む
        const auto& answers = record.getAnswers();
        if (targetQs) {
            for (size_t i = 0; i < targetQs->getQuestionCount(); ++i) {
                file << "質問" << (i + 1) << ": " << targetQs->getQuestion(i) << "\n";
                if (i < answers.size()) {
                    file << "回答: " << answers[i] << "\n\n";
                }
            }
        }

        // ファイルを閉じて、成功（true）を返す
        file.close();
        return true;
    }
    catch (...) {
        // エラーが発生した場合は失敗（false）を返す
        return false;
    }
}