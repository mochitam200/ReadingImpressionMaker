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
    std::string genreName_;              // 書籍のジャンル名
    std::string genreDescription_;       // ジャンルの説明文
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

    // ゲッター
    std::string getGenreName() const { return genreName_; }
    std::string getGenreDescription() const { return genreDescription_; }
    std::vector<std::string> getQuestions() const { return questions_; }

    std::string getQuestion(size_t index) const {
        if (index < questions_.size()) {
            return questions_[index];
        }
        return "";
    }

    size_t getQuestionCount() const { return questions_.size(); }
};

// 読書記録（日付、タイトル、著者名、ジャンル、回答など）を管理するクラス
class ReadingRecord {
private:
    std::string date_;                  // 読了日を保存する変数
    std::string title_;                 // 書籍タイトルを保存する変数
    std::string author_;                // 著者名を保存する変数
    int genreIndex_ = -1;               // ジャンルの選択番号（0始まり） 初期値は未選択(-1)
    std::string genreName_;             // ジャンル名を保存する変数
    std::vector<std::string> answers_;  // 各質問に対する回答を保存する配列

public:
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

    // 入力された日付の形式（YYMMDD, YYMM, YY+季節, 不明）が正しいか判定する関数
    static bool isValidDate(const std::string& d) {
        if (d.empty()) return false;
        if (d == "不明") return true;

        // パターン1: YYMMDD 形式（数字6桁）
        if (d.length() == 6 && std::all_of(d.begin(), d.end(), ::isdigit)) {
            int month = std::stoi(d.substr(2, 2));
            int day = std::stoi(d.substr(4, 2));
            return (month >= 1 && month <= 12 && day >= 1 && day <= 31);
        }

        // パターン2: YYMM 形式（数字4桁）
        if (d.length() == 4 && std::all_of(d.begin(), d.end(), ::isdigit)) {
            int month = std::stoi(d.substr(2, 2));
            return (month >= 1 && month <= 12);
        }

        // パターン3: YY+季節 形式（数字2桁 + 春/夏/秋/冬）
        if (d.length() == 5 && std::isdigit(static_cast<unsigned char>(d[0])) && std::isdigit(static_cast<unsigned char>(d[1]))) {
            std::string season = d.substr(2);
            return (season == "春" || season == "夏" || season == "秋" || season == "冬");
        }

        return false;
    }

    // 文字数（1〜50文字）および「スペースのみでないか」を検証する関数
    static bool isValidStringLength(const std::string& str) {
        if (str.empty()) return false;

        bool allWhitespace = std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return std::isspace(c);
            });
        if (allWhitespace) return false;

        // UTF-8文字数カウント
        size_t charCount = 0;
        for (size_t i = 0; i < str.length();) {
            unsigned char c = static_cast<unsigned char>(str[i]);
            if (c < 0x80) i += 1;
            else if ((c & 0xE0) == 0xC0) i += 2;
            else if ((c & 0xF0) == 0xE0) i += 3;
            else if ((c & 0xF8) == 0xF0) i += 4;
            else i += 1;
            charCount++;
        }
        return (charCount >= 1 && charCount <= 50);
    }

    // setter
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

    void setGenre(int index, const std::string& name) {
        genreIndex_ = index;
        genreName_ = name;
    }

    void setAnswers(const std::vector<std::string>& answers) {
        answers_ = answers;
    }

    void setAnswer(size_t index, const std::string& answer) {
        if (index < answers_.size()) {
            answers_[index] = answer;
        }
    }

    // getter
    std::string getDate() const { return date_; }
    std::string getTitle() const { return title_; }
    std::string getAuthor() const { return author_; }
    int getGenreIndex() const { return genreIndex_; }
    std::string getGenreName() const { return genreName_; }
    std::vector<std::string> getAnswers() const { return answers_; }
    std::string getAnswer(size_t index) const {
        if (index < answers_.size()) return answers_[index];
        return "";
    }
};

#endif // QUESTION_HPP