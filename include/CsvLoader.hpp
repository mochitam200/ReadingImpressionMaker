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

		// 先頭に UTF-8 BOM（0xEF, 0xBB, 0xBF）が含まれている場合、それを取り除く,文字としてではなく、バイトの数値として比較
		if (content.size() >= 3 &&        // content の中身が3バイト以上あるかを確認
			static_cast<unsigned char>(content[0]) == 0xEF &&      // content の1バイト目が EF か確認
			static_cast<unsigned char>(content[1]) == 0xBB &&      // 2バイト目が BB か確認
			static_cast<unsigned char>(content[2]) == 0xBF &&) {   // 3バイト目が BF か確認
			content = content.substr(3);    // ファイルの先頭にUTF-8のBOM（EF BB BF）が付いていたら、先頭3バイトを切り捨てて読み飛ばし
		}
			
		// 改行コード（LF または CRLF）に対応しながら、1行ずつの配列へ分割




	}

};
#endif