#include "../include/CsvLoader.hpp"
#include <fstream>      // ファイル読み込み（std::ifstream）
#include <sstream>      // 文字列分割（std::stringstream）
#include <map>          // 連想配列
#include <algorithm>    // アルゴリズム
#include <cctype>       // 数字判定（std::isdigit）

std::vector<QuestionSet> CsvLoader::load(const std::string& filename) {
    std::vector<QuestionSet> result;

    // BOMの有無や改行コードを正しく処理するため、バイナリモードでファイルを開く
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return result;

    // ファイルの全内容を1つの文字列へ一括で読み込み
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (content.empty()) return result;

    // 先頭に UTF-8 BOM（0xEF, 0xBB, 0xBF）が含まれている場合、それを取り除く
    if (content.size() >= 3 &&
        static_cast<unsigned char>(content[0]) == 0xEF &&
        static_cast<unsigned char>(content[1]) == 0xBB &&
        static_cast<unsigned char>(content[2]) == 0xBF) {
        content = content.substr(3);
    }

    // 改行コード（LF または CRLF）に対応しながら、1行ずつの配列へ分割
    std::vector<std::string> lines;
    std::string currentLine;
    for (size_t i = 0; i < content.size(); i++) {
        char c = content[i];
        if (c == '\r') {
            if (i + 1 < content.size() && content[i + 1] == '\n') {
                i++;
            }
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else if (c == '\n') {
            lines.push_back(currentLine);
            currentLine.clear();
        }
        else {
            currentLine.push_back(c);
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    // ファイル末尾に余分な空行があれば削除
    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }

    // 仕様により、データが30行未満の場合はエラー（空の配列を返す）と判定
    if (lines.size() < 30) return result;

    // ジャンルごとに一時保存する構造体
    struct GenreData {
        std::string name;
        std::string description;
        std::map<int, std::string> questions;
    };
    std::map<int, GenreData> genres;

    // CSVの各行を解析
    for (const auto& line : lines) {
        if (line.empty()) continue;

        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss(line);

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 5) return {};

        auto isNumber = [](const std::string& s) {
            if (s.empty()) return false;
            return std::all_of(s.begin(), s.end(), ::isdigit);
            };

        if (!isNumber(tokens[0]) || !isNumber(tokens[3])) return {};

        int genreNum = std::stoi(tokens[0]);
        std::string genreName = tokens[1];
        std::string genreDescription = tokens[2];
        int qNum = std::stoi(tokens[3]);
        std::string qText = tokens[4];

        genres[genreNum].name = genreName;
        genres[genreNum].description = genreDescription;
        genres[genreNum].questions[qNum] = qText;
    }

    // 整理したマップデータから QuestionSet の配列を組み立てる
    for (const auto& pair : genres) {
        std::vector<std::string> qList;
        for (const auto& qPair : pair.second.questions) {
            qList.push_back(qPair.second);
        }
        result.emplace_back(pair.second.name, pair.second.description, qList);
    }

    return result;
}