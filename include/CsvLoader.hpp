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
	static std::vector<QuestionSet> load(const std::string& filename) { // load 関数にファイル名を渡す
		std::vector<QuestionSet> result;  // 戻り値となる質問セットのリストを用意

		// BOMの有無や改行コードを正しく処理するため、バイナリモードでファイルを開
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open())return result; // ファイルが存在しない・開けない場合は空の配列を返す

		// ファイルの全内容を1つの文字列へ一括で読み込み
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()); // file の現在位置からファイルの最後まで文字を1文字ずつ全部読み込んで content に保存
		file.close(); // ファイルを閉じる

		if (content.empty())return result; // 中身が0バイト（空）なら空の配列を返す

		// 先頭に UTF-8 BOM（0xEF, 0xBB, 0xBF）が含まれている場合、それを取り除く,文字としてではなくバイトの数値として比較
		if (content.size() >= 3 &&        // content の中身が3バイト以上あるかを確認
			static_cast<unsigned char>(content[0]) == 0xEF &&      // content の1バイト目が EF か確認
			static_cast<unsigned char>(content[1]) == 0xBB &&      // 2バイト目が BB か確認
			static_cast<unsigned char>(content[2]) == 0xBF ) {   // 3バイト目が BF か確認
			content = content.substr(3);    // ファイルの先頭にUTF-8のBOM（EF BB BF）が付いていたら、先頭3バイトを切り捨てて読み飛ばし
		}
			
		// 改行コード（LF または CRLF）に対応しながら、1行ずつの配列へ分割
		std::vector<std::string> lines;
		std::string currentLine;
		for (size_t i = 0;i < content.size();i++) {
			char c = content[i];
			if (c == '\r') {
				// Windows形式（\r\n）の場合、次の \n も一緒にスキップ
				if (i + 1 < content.size() && content[i + 1] == '\n') {
					i++;
				}
				lines.push_back(currentLine); // 1行分として登録
				currentLine.clear();// currentLine の中身を消去する
			}
			else {
				currentLine.push_back(c); // 通常の文字は現在の行に追加
			}
		}
		if (!currentLine.empty()) { // 最終行が空行（不要な末尾の空行など）でない場合のみ
			lines.push_back(currentLine); // ファイル末尾の最終行を追加
		}

		// ファイル末尾に余分な空行があれば削除
		while (!lines.empty() && lines.back().empty()) { // リストが空でない & リストの一番後ろの要素が空
			lines.pop_back(); // リストの一番後ろの要素を削除
		}

		// 仕様により、データが30行未満の場合はエラー（空の配列を返す）と判定
		if (lines.size() < 30) return result;

		// ジャンルごとに「ジャンル名」と「質問番号 -> 質問文」を一時保存する構造体
		struct GenreData {
			std::string name;
			std::map<int, std::string> questions; // 問題の番号と問題文のセットを保存する連想配列（マップ）
		};
		std::map<int, GenreData> genres; // ジャンル番号 -> ジャンルデータ(ジャンル番号（キー）とそのジャンルデータ（値）を紐付ける全体の連想配列)

		// CSVの各行を解析
		for (const auto& line : lines) { // lines の中身を1行ずつ取り出して line に入れる
			if (line.empty()) continue; // 空行はスキップ

			std::vector<std::string> tokens;
			std::string token;   // カンマで区切ったときに一時的に1個のデータを入れておく変数
			std::stringstream ss(line); // line の文字列を、文字列ストリームとして扱えるようにする処理

			// カンマ(',')で各カラム（列）に分割
			while (std::getline(ss, token, ',')) { // ss から文字を読み取り、, が出てくるまでを token に入れる
				tokens.push_back(token);  // 読み取った文字を tokens に追加
			}

			// 1行あたりの列数が正確に4列でない場合はエラー（不正なCSV）とみなして空配列を返す
			if (tokens.size() != 4) return {};

			// 文字列が数値のみで構成されているか判定する補助関数
			auto isNumber = [](const std::string& s) {  // s という名前で文字列を受け取る
				if (s.empty()) return false;            // s が空文字列か確認  空文字列なら false…「空文字列そのものを数字として扱わない」ためのチェック
				return std::all_of(s.begin(), s.end(), ::isdigit);  // 文字列 s の最初から最後までが数字か調べ、全部数字なら true、1文字でも数字以外なら false を返す
				// ※ std::all_of が「全部調べる」、::isdigit が「数字か調べる」
				};

			// 1列目（ジャンル番号）と 3列目（質問番号）が数値でない場合はエラー
			if (!isNumber(tokens[0]) || !isNumber(tokens[2])) return {}; // ジャンル番号か質問番号のどちらかが数字ではなかったら、処理を終了

			int genreNum = std::stoi(tokens[0]); // ジャンル番号を数値に変換
			std::string genreName = tokens[1];   // ジャンル名を取得
			int qNum = std::stoi(tokens[2]);     // 質問番号を数値に変換
			std::string qText = tokens[3];       // 質問文を取得

			genres[genreNum].name = genreName;  // そのジャンル番号にジャンル名を保存
			genres[genreNum].questions[qNum] = qText; // そのジャンルの、その質問番号に質問文を保存
		}

		// 整理したマップデータから QuestionSet の配列を組み立てる
		// genres に保存されているジャンルと質問を取り出して、result という「質問セットの一覧」に変換
		for (const auto& pair : genres) {     // genres の中身を1つずつ pair に取り出す
			std::vector<std::string> qList;   // そのジャンルの質問文を一時的に保存する配列
			for (const auto& qPair : pair.second.questions) {   // pair.second.questions の中から質問を1つずつ取り出し
				qList.push_back(qPair.second);     // 質問文を番号順に qList に追加
			}
			result.emplace_back(pair.second.name, qList);   // QuestionSetオブジェクトを作ってresult に登録
		} 

		return result; // 完成した質問セットのリストを返す
	}
};

#endif