#ifndef __TANNERGRAPH_H__
#define __TANNERGRAPH_H__
#pragma once


class CTannerNode
{
public:
	CTannerNode() {};
	CTannerNode(int iID)									{ m_iNodeID = iID; };
	~CTannerNode() {};

	virtual double Compute()								{ return DBL_MIN; };

public:
	int				GetNodeID()										{ return m_iNodeID; }
	void			AddLink(CTannerNode* item)						{ m_pLinks.push_back(item); SetMetric(0.0, 0.0, item->GetNodeID()); };
	int				GetNumLinks()									{ return m_pLinks.size(); };
	CTannerNode*	GetLink(int iLinkID)							{ if (iLinkID < m_pLinks.size()) return m_pLinks[iLinkID]; else return nullptr; };

	std::pair<double, double> GetMetric(int iLinkID)
	{ 
		if (iLinkID < m_pLinks.size())
			return m_metrics[iLinkID];
		else
			return std::pair<double, double>(DBL_MIN, DBL_MIN);
	};

	void SetMetric(double metric_n_m, double metric_m_n, int iDstNodeID )
	{
		std::vector<CTannerNode*>::iterator it = std::find_if(m_pLinks.begin(), m_pLinks.end(), [iDstNodeID](const CTannerNode* x) {return ((CTannerNode*)x)->GetNodeID() == iDstNodeID; });
		std::pair<double, double> prev = m_metrics[std::distance(m_pLinks.begin(), it)];

		if (metric_n_m != DBL_MIN) prev.first = metric_n_m;
		if (metric_m_n != DBL_MIN) prev.second = metric_m_n;
			
		m_metrics[std::distance(m_pLinks.begin(), it)] = prev;
	}
protected:

	// dati relativi alla logica
	//
	std::vector<CTannerNode*>						m_pLinks;
	std::vector<std::pair<double, double>>			m_metrics;

	int							m_iNodeID;
	
	// dati relativi al plot
	cv::Point2f					m_PtCenter;
	cv::Scalar					m_Clr;
};


class CTannerGraph
{
public:
	CTannerGraph(int N, int M);
	~CTannerGraph();

	// Creazione di un link tra nodi
	void CreateLink( int iVNID, int iCNID );

	// Inizializzazione dei nodi variabile
	void Initialization(std::vector<double> channel_data, double sigma_2);
	void CheckNodeUpdate();
	void VariableNodeUpdate();
	bool Decision();

	class TCheckNode : public CTannerNode
	{
	public:
		TCheckNode(int iID) { m_iNodeID = iID; };
	};
	class TVariableNode : public CTannerNode
	{
	public:
		TVariableNode(int iID) { m_iNodeID = iID; };
	};


	void	Draw(cv::Mat mCanvas);

private:

	int							m_iN;
	int							m_iM;
	std::vector<double >		m_dLU;

	std::vector<TCheckNode*>		m_pCheckNodesList;
	std::vector<TVariableNode*>		m_pVariableNodesList;
};

#endif