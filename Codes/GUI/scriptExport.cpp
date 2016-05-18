#include "gwidget.h"
#include "entity.h"
#include "process.h"
#include "edge.h"
#include "mainwindow.h"

QStringList GraphWidget::script() const
{
	QStringList text;
	//set_default();
	
	text.append(s_get_environmental_params()); // load environmental parameters
	text.append("log_file_name = ""log.txt""");
	text.append(s_get_params()); //load charachterictics of unknown parameters *
	text.append(s_get_observed()); // load properties of observed data *
	text.append(s_get_model_configuration()); // load model structure (blocks, connectors)
	text.append(s_get_particle_types()); // load particle types *
	text.append(s_get_constituents()); // load constituent properties *
//	text.append(s_set_default_connector_expressions());
//	text.append(s_set_default_block_expressions());
	text.append(s_get_reactions()); // load reaction network
	text.append(s_get_buildup()); // load buildup properties *
	text.append(s_get_external_flux()); // load External fluxes *
	
//*************	for (int j = 0; j<Precipitation_filename.size(); j++)
//		Precipitation.push_back(CPrecipitation(pathname + Precipitation_filename[j]));
//	for (int j = 0; j<Evaporation_filename.size(); j++)
//		Evaporation.push_back(CBTC(pathname + Evaporation_filename[j]));

/*	for (int i = 0; i<Blocks.size(); i++)
	{
		for (int j = 0; j < Blocks[i].inflow_filename.size(); j++)
			Blocks[i].inflow.push_back(CBTCSet(Blocks[i].inflow_filename[j], 1));


		if ((Blocks[i].indicator == Pond) || (Blocks[i].indicator == Catchment) || (Blocks[i].indicator == Stream))
		{
			if (Blocks[i].precipitation_swch == true)
				for (int j = 0; j<Precipitation_filename.size(); j++)
					Blocks[i].inflow.push_back(Precipitation[j].getflow(Blocks[i].A));
		}
	}
	progress->setValue(37);

*/	
//	text.append(s_load_inflows());
	/*
	vector<int> stds;
	for (int i = 0; i<measured_quan.size(); i++)
	{
		if (lookup(stds, measured_quan[i].std_no) == -1)
		{
			stds.push_back(measured_quan[i].std_no);
			measured_quan[i].std_to_param = int(parameters.size());
			range P;

			P.fixed = false;
			P.log = true;
			P.applytoall = true;
			P.tempcorr = 1;
			P.name = "std_" + numbertostring(i);
			P.low = exp(-4); P.high = exp(4);
			parameters.push_back(P);

		}
	}
	std.resize(stds.size());
	get_funcs();

	

	set_default_params();
	*/
	QStringList r;
	r.append( QString("//Generated Script: ") + OnlyFilename(modelFilename));
	for each (QString line in text)
	{
		QString line1 = line.replace("Normal", "NN!R!M$A$LL");
		line1 = line1.replace("=Yes", "=1").replace("=No", "=0");
		line1 = line1.replace("NN!R!M$A$LL", "Normal");
		r.append(line1);
	}
	return r;
}

QStringList GraphWidget::s_get_params() const
{
	QStringList r;
	r.append("//Unkown Parameters");
	for each (Entity *e in entitiesByType("Parameter"))
	{
		QString p = "Parameter=";
		p.append(e->Name());
		p.append(", {");
		QStringList list = e->codes();
		list.removeOne("Name");
		list.removeOne("log");
		for each (QString code in list)
			//		for each (QString code in e->props.list.keys())
			if (!e->val(code).isEmpty() && e->val(code) != "Default") p.append(QString("%1=%2; ").arg(code).arg(e->val(code)));
		if (e->val("Log") == "Normal")p.append("log=0");
		if (e->val("Log") == "Log-Normal")p.append("log=1");
		if (e->val("Log") == "Uniform")p.append("log=2");
		p.append("}");
		log(p);
		r.append(p);
	}
	return r;
}

QStringList GraphWidget::s_get_observed() const
{
	QStringList r;
	r.append("//Observations");
	for each (Entity *e in entitiesByType("Observed"))
	{
		QString o = "Observed=";
		o.append(e->Name());
		o.append(", {");
		QStringList list = e->codes();
		list.removeOne("Name");
		for each (QString code in list)
//		for each (QString code in e->props.list.keys())
				if (!e->val(code).isEmpty() && e->val(code) != "Default") {
				if (code == "loc_type") o.append(QString("; %1=%2").arg(code).arg((e->val("loc_type") == "Block") ? 0 : 1)); //OBSERVED SUBTYPE
				else if (code == "error_structure") o.append(QString("; %1=%2").arg(code).arg((e->val("error_structure") == "Normal") ? 0 : 1)); //OBSERVED SUBTYPE
				else o.append(QString("; %1=%2").arg(code).arg(e->val(code)));
				o.append("}");
				log(o);
				r.append(o);
			}
	}
	return r;
}

QStringList GraphWidget::s_get_environmental_params() const
{
	QStringList r;
	r.append("//Project/Climate/Solver settings");
	QList<Entity*> list;
	list.append(entitiesByType("Project setting"));
	list.append(entitiesByType("Climate setting"));
	list.append(entitiesByType("Solver setting"));

	for each (Entity *e in list)
		for each (QString key in e->codes())
		{
			if (key == "path")
				r.append(QString("outputpath=%1%2").arg(e->val(key)).arg('/'));
			    //r.append(QString("path=%1%2").arg(e->val(key)).arg('/'));
			else if (key == "forward")
				r.append(QString("forward=%1").arg((e->val(key) == "Forward") ? 1 : 0));
			else if (key == "solution_method")
				r.append(QString("solution_method=%1").arg(e->val(key).toSolutionMethod()));
			else if (e->val(key) != "." && e->val(key) != "Default" && e->val(key) != "")
				r.append(QString("%1=%2").arg(key).arg(e->val(key)));
		}
		
	r.append(QString("detoutfilename_hydro = hydro_output.txt"));
	r.append(QString("detoutfilename_wq = wq_output.txt"));
	r.append(QString("detoutfilename_prtcle = prtcl_output.txt"));
	r.append(QString("detoutfilename_obs = observed_output.txt"));
	//	detout_filename = det_out.txt
	//		detout_obs_filename = obs_det_out.txt;
	//	PE_info_filename = "GA_info.txt";
	return r;
}

QStringList GraphWidget::s_get_model_configuration() const
{
	QStringList r;
	for each(Node *n in Nodes())
	{
		QString b = "Block=";
		b.append(n->objectType.ObjectType);
		b.append(", {id=");
		b.append(n->Name());
//		b.append(QString("; a=%1; depth=%2").arg(n->val("a")).arg(n->val("depth")));
		QStringList list = n->codes();
		list.removeOne("id");
		for each (QString code in list)
		{
			if (code == "inflow" && !n->val(code).isEmpty() && n->val(code) != ".")
				b.append(QString("; inflow=%1").arg(fullFilename(n->val(code), modelPathname()).replace(modelPathname() + "/", "")));
			else if (!n->val(code).isEmpty() && n->val(code) != "Default")
			{
				if (code == "externalflux")
					for each (QString flux in n->val(code).toQString().split(';'))	b.append(QString("; %1=%2").arg(code).arg(flux));
				else
				{
					if (!EntityNames("Parameter").contains(n->val(code)))
						b.append(QString("; %1=%2").arg(code).arg(n->val(code)));
					else
						b.append(QString("; %1=p[%2]").arg(code).arg(n->val(code)));
				}
			}
		}
		if (!n->g().isEmpty()) b.append(";" + n->g());
		if (!n->cg().isEmpty()) b.append(";" + n->cg());

		/*		for each (QString code in n->codes())
				{
				if (EntityNames("Parameter").contains(n->val(code)))
				{
				if (lookup_parameters(n->val(code).toStdString()) != -1) {
				parameters[lookup_parameters(n->val(code).toStdString())].location.push_back(Blocks.size() - 1);  // Check for everything
				parameters[lookup_parameters(n->val(code).toStdString())].quan.push_back(code.toStdString());
				parameters[lookup_parameters(n->val(code).toStdString())].location_type.push_back(0);
				}
				}
				}*/
		b.append("}");
		log(b);
		r.append(b);
	}

	for each(Edge *e in Edges())
	{
		QString c = "Connector=";
		c.append(e->objectType.ObjectType).replace(' ', '_');
		c.append(", {id=");
		c.append(e->Name());
		c.append(QString("; block1=%1; block2=%2").arg(e->sourceNode()->Name()).arg(e->destNode()->Name()));
		QStringList list = e->codes();
		list.removeOne("id");
		for each (QString code in list)
			if (!e->val(code).isEmpty() && e->val(code) != "Default" && e->val(code) != ".")
			{
				if (!EntityNames("Parameter").contains(e->val(code)))
					c.append(QString("; %1=%2").arg(code).arg(e->val(code)));
				else
					c.append(QString("; %1=p[%2]").arg(code).arg(e->val(code)));
			}
		c.append("}");
		log(c);
		r.append(c);
	}
	return r;
}

QStringList GraphWidget::s_get_particle_types() const
{
	QStringList r;
	for each (Entity *e in entitiesByType("Particle"))
	{
		QString p = "Particle=";
		p.append(e->Name());
		p.append(", {model=");
		QString model = e->getValue("Model");
		if (model == "Single Phase") model = "single_phase";
		if (model == "Dual Phase") model = "dual_phase_lm";
		if (model == "Triple Phase") model = "triple_phase_lm";
		p.append(model);
		QStringList list = e->codes();
		list.removeOne("Name");
		list.removeOne("Model");
		for each (QString code in list)
			//		for each (QString code in e->props.list.keys())
			if (!e->val(code).isEmpty() && e->val(code) != "Default")
			{
				if (!EntityNames("Parameter").contains(e->val(code)))
					p.append(QString("; %1=%2").arg(code).arg(e->val(code)));
				else
					p.append(QString("; %1=p[%2]").arg(code).arg(e->val(code)));
			}
		p.append("}");
		log(p);
		r.append(p);
	}
	return r;
}

QStringList GraphWidget::s_get_buildup() const
{
	QStringList r;
	for each (Entity *e in entitiesByType("Build-up"))
	{
		QString b = "Buildup=";
		b.append(e->Name());
		b.append(", {model=");
		b.append(e->getValue("Model"));
		//		if (entityByName(e->val("constituent"))->objectType.ObjectType == "Constituent"){
		//			S.constituent = e->val("constituent").toStdString();
		//			if (e->val("sorb") == "No") S.phase = "aqueous";
		//			else S.phase = "sorbed";
		//		}
		//		else {
		//			S.solid = e->val("constituent").toStdString();
		//			if (e->val("sorb") == "No") S.phase = "mobile";
		//			else S.phase = "attached";
		//		}
		QStringList list = e->codes();
		list.removeOne("Name");
		list.removeOne("Model");
		for each (QString code in list)
			//		for each (QString code in e->props.list.keys())
				if (!e->val(code).isEmpty()) b.append(QString("; %1=%2").arg(code).arg(e->val(code)));
		b.append("}");
		log(b);
		r.append(b);
	}
	return r;
}

QStringList GraphWidget::s_get_external_flux() const
{
	QStringList r;
	for each (Entity *e in entitiesByType("External flux"))
	{
		QString f = "externalflux=";
		f.append(e->Name());
		f.append(", {model=");
					
		QString model = e->getValue("Model").toLower();
		if (model == "free surface") model = "free_surface";
		if (model == "constant influx") model = "constant_influx";
		if (model == "constant rate") model = "constant_rate";

		f.append(model);
		QStringList list = e->codes();
		list.removeOne("Name");
		list.removeOne("Model");
		for each (QString code in list)
			//		for each (QString code in e->props.list.keys())
			if (!e->val(code).isEmpty() && e->val(code) != "Default")
			{
				if (!EntityNames("Parameter").contains(e->val(code)))
					f.append(QString("; %1=%2").arg(code).arg(e->val(code)));
				else
					f.append(QString("; %1=p[%2]").arg(code).arg(e->val(code)));
			}
		f.append("}");
		log(f);
		r.append(f);
	}
	return r;
}

QStringList GraphWidget::s_get_constituents() const
{
	QStringList r;
	for each (Entity *e in entitiesByType("Constituent"))
	{
		QString c = "Constituent=";
		c.append(e->Name());
		bool empty = true;
		QStringList list = e->codes();
		list.removeOne("Name");
		for each (QString code in list)
			//		for each (QString code in e->props.list.keys())
		{
			if (empty)
				if (!e->val(code).isEmpty() && e->val(code) != "Default")
					if (!e->val(code).isEmpty() && e->val(code) != "Default")
					{
						if (!EntityNames("Parameter").contains(e->val(code)))
							c.append(QString(", {%1=%2").arg(code).arg(e->val(code)));
						else
							c.append(QString(", {%1=p[%2]").arg(code).arg(e->val(code)));
						empty = false;
					}
			if (!empty)
				if (!e->val(code).isEmpty() && e->val(code) != "Default")
				{
					if (!EntityNames("Parameter").contains(e->val(code)))
						c.append(QString("; %1=%2").arg(code).arg(e->val(code)));
					else
						c.append(QString("; %1=p[%2]").arg(code).arg(e->val(code)));
				}
		}

		if (!empty) c.append("}");
		log(c);
		r.append(c);
	}
	return r;
}

QStringList GraphWidget::s_get_reactions() const
{
	QStringList r;
	for each (Entity *e in entitiesByType("Reaction parameter"))
	{
		QString p = "Reaction_parameter=";
		p.append(e->Name());
		p.append(", {");
		p.append(QString("value=%2").arg(e->val("value")));
		QStringList list = e->codes();
		list.removeOne("Name");
		list.removeOne("value");

		for each (QString code in list)
			//		for each (QString code in e->props.list.keys())
			if (!e->val(code).isEmpty() && e->val(code) != "Default")
			{
				if (!EntityNames("Parameter").contains(e->val(code)))
					p.append(QString("; %1=%2").arg(code).arg(e->val(code)));
				else
					p.append(QString("; %1=p[%2]").arg(code).arg(e->val(code)));
			}
		p.append("}");
		log(p);
		r.append(p);
	}
	for each (Process *e in Processes)
	{
		QString p = "Reaction=";
		p.append(e->name);
		p.append(", {rate=");
		p.append(e->rateExpression());

		for each (QString key in e->productExpressions().keys())
			p.append(QString("; %1").arg(e->productExpressions()[key]));
		//			p.append(QString("; product[%1]=%2").arg(key).arg(e->productExpressions()[key]));
		p.append("}");
		log(p);
		r.append(p);
	}
	return r;
}
