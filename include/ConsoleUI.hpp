#pragma once
#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include "Question.hpp" // ReadingRecordやQuestionSetを使うため
#include <string>       // 文字列を扱うため
#include <vector>       // 配列を扱うため

// 画面入力の処理をまとめたクラス
class ConsoleUI {
public:
    // 正しい読了日（YYMMDD/YYMM/YY+季節/不明）が入力されるまで繰り返し入力を受け取り（未入力時は本日日付）
    static std::string inputDate();

    // 正しいタイトル（1〜50文字、全空白でない）が入力されるまで繰り返し読み込み（未入力・空白のみ時はUnknown_title）
    static std::string inputTitle();

    // 正しい著者名（1〜50文字、全空白でない）が入力されるまで繰り返し読み込み（未入力・空白のみ時はUnknown_author）
    static std::string inputAuthor();

    // ジャンル選択番号を受け取ってインデックス番号（0始まり）を返す
    static int selectGenre(const std::vector<QuestionSet>& questions);

    // 質問ごとにユーザーからの回答入力を収集する関数
    static std::vector<std::string> askQuestions(const QuestionSet& questionSet);
};

#endif // CONSOLE_UI_HPP