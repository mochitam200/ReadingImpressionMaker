#pragma once
#ifndef CSV_LOADER_HPP
#define CSV_LOADER_HPP

#include "Question.hpp" // QuestionSetクラスを利用するため
#include <string>       // 文字列を扱うため
#include <vector>       // 配列を扱うため
#include <fstream>      // ファイルの読み込み（std::ifstream）を行うため
#include <sstream>      // 文字列の分割（std::stringstream）に便利
#include <map>          // キーと値の組み合わせ（連想配列）を使うため
#include <algorithm>    // 条件チェックなどのアルゴリズム
#include <cctype>       // 数字判定（std::isdigit）

// CSVファイルから質問一覧を読み取るクラス
class CsvLoader {
public:
	// 指定されたファイル名からCSVを読み込み、QuestionSetのリストにして返す静的関数
	static std::vector<QuestionSet> load(const std::string& filename) {
		std::vector<QuestionSet> result;  // 戻り値となる質問セットのリストを用意

		// BOMの有無や改行コードを正しく処理するため、バイナリモードでファイルを開く
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open()) return result; // ファイルが存在しない・開けない場合は空の配列を返す

		// ファイルの全内容を1つの文字列へ一括で読み込み
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close(); // ファイルを閉じる

		if (content.empty()) return result; // 中身が0バイト（空）なら空の配列を返す

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
				// Windows形式（\r\n）の場合、次の \n も一緒にスキップ
				if (i + 1 < content.size() && content[i + 1] == '\n') {
					i++;
				}
				lines.push_back(currentLine); // 1行分として登録
				currentLine.clear();
			}
			else if (c == '\n') { // LF単体の改行にも対応
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

		// ジャンルごとに「ジャンル名」「ジャンル説明」「質問番号 -> 質問文」を一時保存する構造体
		struct GenreData {
			std::string name;
			std::string description;
			std::map<int, std::string> questions;
		};
		std::map<int, GenreData> genres;

		// CSVの各行を解析
		for (const auto& line : lines) {
			if (line.empty()) continue; // 空行はスキップ

			std::vector<std::string> tokens;
			std::string token;
			std::stringstream ss(line);

			// カンマ(',')で各カラム（列）に分割
			while (std::getline(ss, token, ',')) {
				tokens.push_back(token);
			}

			// 1行あたりの列数が正確に5列でない場合はエラー（不正なCSV）とみなして空配列を返す
			if (tokens.size() != 5) return {};

			// 文字列が数値のみで構成されているか判定する補助関数
			auto isNumber = [](const std::string& s) {
				if (s.empty()) return false;
				return std::all_of(s.begin(), s.end(), ::isdigit);
				};

			// 1列目（ジャンル番号）と 4列目（質問番号）が数値でない場合はエラー
			if (!isNumber(tokens[0]) || !isNumber(tokens[3])) return {};

			int genreNum = std::stoi(tokens[0]);            // ジャンル番号を数値に変換
			std::string genreName = tokens[1];              // ジャンル名を取得
			std::string genreDescription = tokens[2];       // ジャンル説明を取得
			int qNum = std::stoi(tokens[3]);                // 質問番号を数値に変換
			std::string qText = tokens[4];                  // 質問文を取得

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

		return result; // 完成した質問セットのリストを返す
	}
};

#endif