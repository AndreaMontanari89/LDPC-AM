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
	void			AddLink(CTannerNode* item)						{ m_pLinks.push_back(item); m_metrics.push_back(0.0); };
	int				GetNumLinks()									{ return m_pLinks.size(); };
	CTannerNode*	GetLink(int iLinkID)							{ if (iLinkID < m_pLinks.size()) return m_pLinks[iLinkID]; else return nullptr; };

	double GetMetric(int iLinkID)
	{ 
		double dRetVal = DBL_MIN;
		if (iLinkID < m_pLinks.size())
			dRetVal = m_metrics[iLinkID];
		else
			dRetVal = DBL_MIN;

		return dRetVal;
	};

	double GetNodeMetric( int iDstNode ) // torna la metrica al lato del nodo link con id iDstNode
	{
		double dRetVal = DBL_MIN;
		for (int l = 0; l < m_pLinks.size(); l++)
		{
			CTannerNode* pDstNode = m_pLinks[l];
			if (pDstNode->GetNodeID() != iDstNode)
				continue;

			for (int k = 0; k < pDstNode->GetNumLinks(); k++)
			{
				if (pDstNode->GetLink(k)->GetNodeID() == m_iNodeID)
				{
					dRetVal = pDstNode->GetMetric(k);
				}
			}
		}

		return dRetVal;
	};

	void SetMetric(int iLinkID, double metric )
	{
		if (iLinkID < m_pLinks.size())
			m_metrics[iLinkID] = metric;
	}

protected:

	// dati relativi alla logica
	//
	std::vector<CTannerNode*>	m_pLinks;
	std::vector<double>			m_metrics;

	int							m_iNodeID;
	
	// dati relativi al plot
	cv::Point2f					m_PtCenter;
	cv::Scalar					m_Clr;
};


class CTannerGraph
{
public:
	CTannerGraph(cv::Mat pchk);
	~CTannerGraph();

	// Creazione di un link tra nodi
	void CreateLink( int iVNID, int iCNID );

	// Inizializzazione dei nodi variabile
	void Initialization(std::vector<double> channel_data, double sigma_2);
	void CheckNodeUpdate();
	void VariableNodeUpdate();
	bool Decision(cv::Mat pchk, wxString& cw);
	wxString Decode(std::vector<double> channel_data, double sigma_2, cv::Mat pchk, int iAttempt);

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