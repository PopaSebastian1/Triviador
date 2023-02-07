#pragma once

#include <QMainWindow>
#include "ui_Play.h"
#include <cpr/cpr.h>
#include <crow.h>
#include <string>
class Play : public QMainWindow
{
	Q_OBJECT

public:
	Play(QWidget* parent = nullptr, std::string username = "");
	~Play();
public slots:
	void on_AnswerA_clicked();
	void on_AnswerB_clicked();
	void on_AnswerC_clicked();
	void on_AnswerD_clicked();
	void updateProgressBar();
	void close1();
	void on_AvantajJumatate_clicked();
	void on_AvantajAlegereRaspuns_clicked();
	void on_AvantajSugestieRaspuns_clicked();
	void on_AvantajSchimbareRaspuns_clicked();

	//void on_NumericAnswer_typed();
private:
	Ui::ClasaPlay ui;
	std::string m_username;
	std::string correctAnswer;
	bool okAvantajJumatate = false;
	bool okAvantajAlegere = false;
	bool okAvantajSugestie = false;
	bool okAvantajSchimbare = false;

};