#pragma once
#ifndef CSV_LOADER_HPP
#define CSV_LOADER_HPP

#include "Question.hpp" // QuestionSetクラスを利用するため
#include <string>       // 文字列を扱うため
#include <vector>       // 配列を扱うため

// CSVファイルから質問一覧を読み取るクラス
class CsvLoader {
public:
    // 指定されたファイル名からCSVを読み込み、QuestionSetのリストにして返す静的関数
    static std::vector<QuestionSet> load(const std::string& filename);
};

#endif // CSV_LOADER_HPP