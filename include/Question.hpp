#pragma once

#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>    // 文字列（std::string）を扱うため
#include <vector>    // 可変長配列（std::vector）を扱うため
#include <algorithm> // 全要素判定（std::all_of）などの便利なアルゴリズムを使うため
#include <cctype>    // 数字判定（std::isdigit）や空白判定（std::isspace）に使う

// 質問セットを保持するクラス(ジャンル名と質問文の配列を持つ)
class QuestionSet {
private:
	std::string genreName_;      // 書籍のジャンル名
	std::vector<std::string> questions_;  // 質問文の配列(4問)

public:
	// デフォルトンストラクタ(空のオブジェクト)
	QuestionSet() = default;

	// ジャンル名と質問リストを受け取って初期化するコンストラクタ
	QuestionSet(const std::string& genreName,const std::vector<std::string>& questions)
		:genreName_(genreName),questions_(questions){ }

	// ジャンル名を取得するゲッター
	std::string getGenreName() const { return genreName_; }

	// 質問リストをすべて取得するゲッター関数
	std::vector<std::string> getQuestions() const { return questions_; }

	// 指定した番号（0始まりのインデックス）の質問文を取得する関数
	std::string getQuestion(size_t index) const { 
		if (index < questions_.size()) { // 指定された番号が配列の範囲内か確認
			return questions_[index]; // 範囲内ならその質問文を返します。
		}
		return ""; // 範囲外なら空文字を返します。
	}

	// 質問の総数を取得する関数
	size_t getQuestionCount() const { return questions_.size(); }	

};

// 読書記録（日付、タイトル、著者名、ジャンル、回答など）を管理するクラス
class ReadingRecord {
private:
	std::string date_;         // 読了日を保存する変数
	std::string title_;        // 書籍タイトルを保存する変数
	std::string author;        // 著者名を保存する変数
	int genreIndex_ = -1;           // ジャンルの選択番号（0始まり） 初期値は未選択(-1)
	std::string genleName_;    // ジャンル名を保存する変数
	std::vector<std::string> answers_;    // 各質問に対する回答を保存する配列

public:
	// デフォルトコンストラクタ
	ReadingRecord() = default;

	// 入力された日付の形式（YYMMDD, YYMM, YY+季節）が正しいか判定する関数
	static bool isValidDate(const std::string& d) {
		if (d.empty()) return false; // 空文字は不合格とする
		
		// パターン1: YYMMDD 形式（数字6桁）
		if (d.length() == 6 && std::all_of(d.begin(), d.end(), ::isdigit)) { // isdigit…その文字が数字（0〜9）かどうかを判定する関数
			int month = std::stoi(d.substr(2,2));     // 3〜4文字目（月）を数値に変換、substr（サブストリング）…文字列の中から指定した一部分を切り出す
			int day = std::stoi(d.substr(4,2));       // 5〜6文字目（日）を数値に変換
			return (month >= 1 && month <= 12 && day >= 1 && day <= 31);  // 月と日の範囲を検証
		}

		// パターン2: YYMM 形式（数字4桁）
		if (d.length() == 4 && std::all_of(d.begin(), d.end(), ::isdigit)) {
			int month = std::stoi(d.substr(2, 2));    // 3〜4文字目（月）を数値に変換
			return(month >= 1 && month <= 12);        // 月の範囲を検証
		}

		// パターン3: YY+季節 形式（数字2桁 + 春/夏/秋/冬）
		if (d.length() == 5 && std::isdigit(d[0]) && isdigit(d[1])) {
			std::string season = d.substr(2);  // 3文字目以降（季節の文字列）を切り出し
			return(season == "春" || season == "夏" || season == "秋" || season == "冬"); // 月の範囲を検証
		}

		return false; // どの形式にも一致しない場合は不正とする
	}

	// タイトルや著者名の文字数（1〜50文字）および「スペースのみでないか」を検証する関数
	// isValidStringLength…文字列の長さが指定した範囲内にあるかを調べるための判定用関数
	static bool isValidStringLength(const std::string& str) { // 文字列 str の長さが正しいかを判定
		if (str.empty())return false;  // 空文字は不合格

		// 入力された文字列が「すべて空白文字（スペースやタブなど）」でないか確認
		bool allWhitespace = std::all_of(str.begin(), str.end(), [](unsigned char c) { // unsigned…「符号なし」のデータ型を指定
			return std::isspace(c);
		});
		if (allWhitespace)return false;  // スペースだけの場合は不合格

		// UTF-8エンコーディングにおける実際の「文字数」をカウント（日本語の全角対応）
		size_t charCount = 0;
		for (size_t i = 0;i < str.length();) {
			unsigned char c = static_cast<unsigned char>(str[i]);  // str の i 番目の文字を unsigned char 型に変換して、c に入れている
			if (c < 0x80)i += 1;                  // 1バイト文字（半角英数字など）,0x80 は16進数,10進数で128
			else if ((c & 0xE0) == 0xC0) i += 2;  // 2バイト文字
			else if ((c & 0xF0) == 0xE0) i += 3;  // 3バイト文字（一般的な日本語）
			else if ((c & 0xF8) == 0xF0) i += 4;  // 4バイト文字（絵文字など）
			else i += 1;         // 上記の文字以外
			charCount++; // 1文字としてカウントします。
		}
		return (charCount >= 1 && charCount <= 50); // 1文字以上50文字以内かチェック
	}

	// 読了日を設定（形式チェックを行い、成功したかどうかをboolで返す）






};



#endif