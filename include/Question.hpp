#pragma once

#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>    // 文字列（std::string）を扱うため
#include <vector>    // 可変長配列（std::vector）を扱うため
#include <algorithm> // 全要素判定（std::all_of）などの便利なアルゴリズムを使うため
#include <cctype>    // 数字判定（std::isdigit）や空白判定（std::isspace）に使う
#include <ctime>     // 本日日付の自動取得（std::time, std::strftime）に使う

// 質問セットを保持するクラス(ジャンル名、ジャンル説明、質問文の配列を持つ)
class QuestionSet {
private:
	std::string genreName_;             // 書籍のジャンル名
	std::string genreDescription_;      // ジャンルの説明文
	std::vector<std::string> questions_; // 質問文の配列

public:
	// デフォルトコンストラクタ(空のオブジェクト)
	QuestionSet() = default;

	// ジャンル名と質問リストを受け取って初期化するコンストラクタ
	QuestionSet(const std::string& genreName, const std::vector<std::string>& questions)
		: genreName_(genreName), genreDescription_(""), questions_(questions) {
	}

	// ジャンル名、説明文、質問リストを受け取って初期化するコンストラクタ
	QuestionSet(const std::string& genreName, const std::string& genreDescription, const std::vector<std::string>& questions)
		: genreName_(genreName), genreDescription_(genreDescription), questions_(questions) {
	}

	// ジャンル名を取得するゲッター
	std::string getGenreName() const { return genreName_; }

	// ジャンル説明文を取得するゲッター
	std::string getGenreDescription() const { return genreDescription_; }

	// 質問リストをすべて取得するゲッター関数
	std::vector<std::string> getQuestions() const { return questions_; }

	// 指定した番号（0始まりのインデックス）の質問文を取得する関数
	std::string getQuestion(size_t index) const {
		if (index < questions_.size()) { // 指定された番号が配列の範囲内か確認
			return questions_[index]; // 範囲内ならその質問文を返す
		}
		return ""; // 範囲外なら空文字を返す
	}

	// 質問の総数を取得する関数
	size_t getQuestionCount() const { return questions_.size(); }
};

// 読書記録（日付、タイトル、著者名、ジャンル、回答など）を管理するクラス
class ReadingRecord {
private:
	std::string date_;         // 読了日を保存する変数
	std::string title_;        // 書籍タイトルを保存する変数
	std::string author_;       // 著者名を保存する変数
	int genreIndex_ = -1;      // ジャンルの選択番号（0始まり） 初期値は未選択(-1)
	std::string genreName_;    // ジャンル名を保存する変数
	std::vector<std::string> answers_; // 各質問に対する回答を保存する配列

public:
	// デフォルトコンストラクタ
	ReadingRecord() = default;

	// 本日の日付を YYMMDD 形式（6桁数字）で取得する補助関数
	static std::string getTodayDate() {
		std::time_t t = std::time(nullptr);
		std::tm tm{};
#if defined(_WIN32) || defined(_WIN64)
		localtime_s(&tm, &t);
#else
		localtime_r(&t, &tm);
#endif
		char buf[7];
		std::strftime(buf, sizeof(buf), "%y%m%d", &tm);
		return std::string(buf);
	}

	// 入力された日付の形式（YYMMDD, YYMM, YY+季節）が正しいか判定する関数
	static bool isValidDate(const std::string& d) {
		if (d.empty()) return false; // 空文字は不合格とする

		// パターン4: 不明（読了時期不明）
		if (d == "不明") return true;

		// パターン1: YYMMDD 形式（数字6桁）
		if (d.length() == 6 && std::all_of(d.begin(), d.end(), ::isdigit)) { // isdigit…その文字が数字（0〜9）かどうかを判定する関数
			int month = std::stoi(d.substr(2, 2));    // 3〜4文字目（月）を数値に変換
			int day = std::stoi(d.substr(4, 2));      // 5〜6文字目（日）を数値に変換
			return (month >= 1 && month <= 12 && day >= 1 && day <= 31);  // 月と日の範囲を検証
		}

		// パターン2: YYMM 形式（数字4桁）
		if (d.length() == 4 && std::all_of(d.begin(), d.end(), ::isdigit)) {
			int month = std::stoi(d.substr(2, 2));    // 3〜4文字目（月）を数値に変換
			return (month >= 1 && month <= 12);       // 月の範囲を検証
		}

		// パターン3: YY+季節 形式（数字2桁 + 春/夏/秋/冬）
		if (d.length() == 5 && std::isdigit(static_cast<unsigned char>(d[0])) && std::isdigit(static_cast<unsigned char>(d[1]))) {
			std::string season = d.substr(2);  // 3文字目以降（季節の文字列）を切り出し
			return (season == "春" || season == "夏" || season == "秋" || season == "冬"); // 季節の文字列を検証
		}

		return false; // どの形式にも一致しない場合は不正とする
	}

	// タイトルや著者名の文字数（1〜50文字）および「スペースのみでないか」を検証する関数
	static bool isValidStringLength(const std::string& str) {
		if (str.empty()) return false;  // 空文字は不合格

		// 入力された文字列が「すべて空白文字（スペースやタブなど）」でないか確認
		bool allWhitespace = std::all_of(str.begin(), str.end(), [](unsigned char c) {
			return std::isspace(c);
			});
		if (allWhitespace) return false;  // スペースだけの場合は不合格

		// UTF-8エンコーディングにおける実際の「文字数」をカウント（日本語の全角対応）
		size_t charCount = 0;
		for (size_t i = 0; i < str.length();) {
			unsigned char c = static_cast<unsigned char>(str[i]);
			if (c < 0x80) i += 1;                  // 1バイト文字（半角英数字など）
			else if ((c & 0xE0) == 0xC0) i += 2;  // 2バイト文字
			else if ((c & 0xF0) == 0xE0) i += 3;  // 3バイト文字（一般的な日本語）
			else if ((c & 0xF8) == 0xF0) i += 4;  // 4バイト文字（絵文字など）
			else i += 1;                           // 上記の文字以外
			charCount++;
		}
		return (charCount >= 1 && charCount <= 50); // 1文字以上50文字以内かチェック
	}

	// 読了日を設定（形式チェックを行い、成功したかどうかをboolで返す。空文字時は本日日付で自動補完）
	bool setDate(const std::string& date) {
		if (date.empty()) {
			date_ = getTodayDate();
			return true;
		}
		if (isValidDate(date)) {
			date_ = date;
			return true;
		}
		return false;
	}

	// 読了日を取得
	std::string getDate() const { return date_; }

	// タイトルを設定（空文字時は Unknown_title で自動補完）
	bool setTitle(const std::string& title) {
		if (title.empty()) {
			title_ = "Unknown_title";
			return true;
		}
		if (isValidStringLength(title)) {
			title_ = title;
			return true;
		}
		return false;
	}

	// タイトルを取得
	std::string getTitle() const { return title_; }

	// 著者名を設定（空文字時は Unknown_author で自動補完）
	bool setAuthor(const std::string& author) {
		if (author.empty()) {
			author_ = "Unknown_author";
			return true;
		}
		if (isValidStringLength(author)) {
			author_ = author;
			return true;
		}
		return false;
	}

	// 著者名を取得
	std::string getAuthor() const { return author_; }

	// ジャンル番号（インデックス）とジャンル名を設定
	void setGenre(int index, const std::string& name) {
		genreIndex_ = index;
		genreName_ = name;
	}

	// ジャンルの選択番号を取得
	int getGenreIndex() const { return genreIndex_; }

	// ジャンル名を取得
	std::string getGenreName() const { return genreName_; }

	// 回答のリストをまとめて設定
	void setAnswers(const std::vector<std::string>& answers) {
		answers_ = answers;
	}

	// 回答のリストをすべて取得
	std::vector<std::string> getAnswers() const { return answers_; }

	// 特定の質問（インデックス指定）の回答だけを差し替え
	void setAnswer(size_t index, const std::string& answer) {
		if (index < answers_.size()) {
			answers_[index] = answer;
		}
	}

	// 特定の質問の回答を取得
	std::string getAnswer(size_t index) const {
		if (index < answers_.size()) return answers_[index];
		return "";
	}
};

#endif