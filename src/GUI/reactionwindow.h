#ifdef GIFMOD
#ifndef REACTIONWINDOW_H
#define REACTIONWINDOW_H

#include <QMainWindow>
class GraphWidget;

struct ReactionWindowPri
{
	QStringList Constituents, Parameters, Functions, Physical;

	ReactionWindowPri()
	{
		Constituents << "Si" << "Ss" << "Sm" << "Xi" << "Xs" << "Xbh" << "Xbm" << "Xba" // Constituents
			<< "Xp" << "So" << "Sno" << "Snh" << "Snd" << "Xnd" << "Salk"
			<< "X_s" << "X_bh" << "Kx" << "" << "K_oh" << "S_o" << "et_h" << "S_no" << "K_no";

		Parameters << "mu_H" << "K_s" << "K_MH" << "K_OH" << "K_NOH" << "eta_g" << "b_H" << "K_NH" // params - Heterotrophic kinetics:
			<< "mu_M" << "K_MM" << "K_OM" << "K_NOM" << "b_M" // Methlotrophic kinetics
			<< "mu_A" << "K_NHA" << "K_NOA" << "K_OA" << "b_A" // Autotrophic kinetics
			<< "eta_h" << "k_h" << "k_x" << "k_a" // Conversion kinetics
			<< "Y_H" << "Y_HM" << "Y_A" << "Y_M" << "f_p" // Stoichiometeries
			<< "i_XB" << "i_XP" << "i_NOxN2" << "i_CODNOx" << "f_SiCODs" << "i_CV_Bio" << "i_CV_i" << "i_CV_s" << "i_CV_p" << "f_Ss_Snd" << "K_fake"; // Partitioning Parameters

		Functions << "exp" << "hsd" << "min" << "max" << "lne" << "lnt" << "sgm" << "pos" << "sq1" << "sqr" // functions
			<< "frs" << "fas" << "ply" << "mon" << "sq2" << "abs";

		Physical << "area" << "light" << "temperature";
	}
};


class ReactionWindow : public QMainWindow
{
    Q_OBJECT
public:
	explicit ReactionWindow(QWidget *parent = 0);
	explicit ReactionWindow(GraphWidget *gWidget, QWidget *parent = 0);
	~ReactionWindow();
	GraphWidget *mainGraphWidget;

private slots:
    void onExpressionConverted(const QList<QStringList>& data);

private:
    ReactionWindowPri* d;
};

#endif // REACTIONWINDOW_H
#endif