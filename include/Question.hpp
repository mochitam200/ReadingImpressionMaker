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




};



#endif