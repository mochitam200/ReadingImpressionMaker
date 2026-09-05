#pragma once
#ifndef RECORD_EXPORTER_HPP
#define RECORD_EXPORTER_HPP

#include "Question.hpp" // ReadingRecordやQuestionSetを使うため
#include <string>       // 文字列を扱うため
#include <vector>       // 配列を扱うため

// ファイルへの書き出し処理を行うクラス
class RecordExporter {
private:
    // OSのファイル名で使用できない禁止文字（\ / : * ? " < > |）を '_'（アンダースコア）に置換する関数
    static std::string sanitizeFilename(const std::string& name);

public:
    // 読書記録と質問セットを受け取り、指定されたフォーマットでテキストファイルを出力
    static bool exportToFile(const ReadingRecord& record, const std::vector<QuestionSet>& questions);
};

#endif // RECORD_EXPORTER_HPP