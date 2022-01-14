#include "stdafx.h"
#include "TannerGraph.h"

CTannerGraph::CTannerGraph(cv::Mat pchk)
{
	m_iN = pchk.cols;
	m_iM = pchk.rows;
	// istanzio tutti i nodi
	for (int i = 0; i < m_iN; i++)
		m_pVariableNodesList.push_back(new TVariableNode(i+1));

	for (int i = 0; i < m_iM; i++)
		m_pCheckNodesList.push_back(new TCheckNode(i+1));

	for (int r = 0; r < pchk.rows; r++)
	{
		for (int c = 0; c < pchk.cols; c++)
		{	
			if (pchk.at<float>(r,c) != 0.0 )
				CreateLink(c+1, r+1);
		}
	}
}

CTannerGraph::~CTannerGraph()
{

}

void CTannerGraph::CreateLink(int iVNID, int iCNID)
{
	do
	{
		if (iVNID > m_iN || iCNID > m_iM)
			break;

		// Ricavo i puntatori ai 2 nodi
		std::vector<TVariableNode*>::iterator itV = std::find_if(m_pVariableNodesList.begin(), m_pVariableNodesList.end(), [iVNID](const TVariableNode* x) {return ((CTannerNode*)x)->GetNodeID() == iVNID; });		
		std::vector<TCheckNode*>::iterator itC = std::find_if(m_pCheckNodesList.begin(), m_pCheckNodesList.end(), [iCNID](const TCheckNode* x) {return ((CTannerNode*)x)->GetNodeID() == iCNID; });

		// Creo il collegamento
		CTannerNode* VN = (*itV);
		CTannerNode* CN = (*itC);
		VN->AddLink(CN);
		CN->AddLink(VN);

	} while(0);
}

void CTannerGraph::Initialization( std::vector<double> channel_data, double sigma_2 )
{
	do
	{
		if (channel_data.size() != m_iN)
			break;
		m_dLU.clear();
		for (int i = 0; i < m_iN; i++)
			m_dLU.push_back(2 * channel_data[i] / sigma_2);

		for (int c = 0; c < m_iM; c++)
			for (int k = 0; k < m_pCheckNodesList[c]->GetNumLinks(); k++)				
				m_pCheckNodesList[c]->SetMetric( k, 0.0 );

		for (int v = 0; v < m_iN; v++)
			for (int k = 0; k < m_pVariableNodesList[v]->GetNumLinks(); k++)
				m_pVariableNodesList[v]->SetMetric( k, m_dLU[v]);

	} while (0);
}

void CTannerGraph::CheckNodeUpdate()
{
	
	for (int c = 0; c < m_iM; c++) // Ciclo su tutti i nodi check
	{
		int link_m = m_pCheckNodesList[c]->GetNodeID();  // indice che mi rappresente il check node corrente sul grafo

		for (int iLinkIdx = 0; iLinkIdx < m_pCheckNodesList[c]->GetNumLinks(); iLinkIdx++) // ciclo su tutti i link ai variable node
		{
			int link_n = m_pCheckNodesList[c]->GetLink(iLinkIdx)->GetNodeID(); // indice che mi rappresente il variable node di cui calcolo la metrica
			double dAccum = 1.0;			

			for (int n = 0; n < m_pCheckNodesList[c]->GetNumLinks(); n++)  // ciclo sui variable node diversi dal VN corrente 
			{						
				int link_n1 = m_pCheckNodesList[c]->GetLink(n)->GetNodeID();

				if (link_n1 != link_n)
				{					
					// ricavo la metrica lato VN di quel link
					double metrics = m_pCheckNodesList[c]->GetNodeMetric(link_n1);
					dAccum *= tanh(metrics / 2.0);
				}

			}
			m_pCheckNodesList[c]->SetMetric(iLinkIdx, 2*atanh(dAccum)); //assegna la nuova metrica al check node
		}		
	}
		
}

void CTannerGraph::VariableNodeUpdate()
{
	for (int v = 0; v < m_iN; v++) // ciclo su tutti nodi variable 
	{
		int link_n = m_pVariableNodesList[v]->GetNodeID();  // indice che mi rappresente il variable node corrente sul grafo

		for (int iLinkIdx = 0; iLinkIdx < m_pVariableNodesList[v]->GetNumLinks(); iLinkIdx++) // ciclo su tutti i link ai check node
		{
			int link_m = m_pVariableNodesList[v]->GetLink(iLinkIdx)->GetNodeID(); // indice che mi rappresente il variable node di cui calcolo la metrica
			double dAccum = 0.0;

			for (int m = 0; m < m_pVariableNodesList[v]->GetNumLinks(); m++)
			{
				int link_m1 = m_pVariableNodesList[v]->GetLink(m)->GetNodeID();

				if (link_m1 != link_m)
				{
					// ricavo la metrica lato CN di quel link
					double metrics = m_pVariableNodesList[v]->GetNodeMetric(link_m1);
					dAccum += metrics;
				}

			}
			m_pVariableNodesList[v]->SetMetric(iLinkIdx, m_dLU[v]+dAccum);
		}
	}
}

bool CTannerGraph::Decision(cv::Mat pchk, wxString& cw)
{
	bool bRetVal = false;
	std::vector<double> result;
	for (int v = 0; v < m_iN; v++)
	{
		int link_n = m_pVariableNodesList[v]->GetNodeID();  // indice che mi rappresente il variable node corrente sul grafo
		double dAccum = 0.0;

		for (int iLinkIdx = 0; iLinkIdx < m_pVariableNodesList[v]->GetNumLinks(); iLinkIdx++) // ciclo su tutti i link ai check node
		{	

			int link_m = m_pVariableNodesList[v]->GetLink(iLinkIdx)->GetNodeID(); // indice che mi rappresente il variable node di cui calcolo la metrica
			double metric = m_pVariableNodesList[v]->GetNodeMetric(link_m);
			dAccum += metric;
		}

		result.push_back(m_dLU[v] + dAccum);
	}

	cv::Mat word = cv::Mat::zeros(1, m_iN, CV_32FC1);
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i] >= 0)
			word.at<float>(0, i) = 1.0;
	}

	cv::Mat res = word * pchk.t();

	res.convertTo(res, CV_8UC1);

	for (int i = 0; i < word.cols; i++)
		if (word.at<float>(0, i) == 1.0) cw += "1"; else cw += "0";

	for (int i = 0; i < res.cols; i++)
		if (res.at<uchar>(0, i) % 2 == 0) res.at<uchar>(0, i) = 0;

	if (cv::countNonZero(res) == 0)
		bRetVal = true;


	return bRetVal;
}

wxString CTannerGraph::Decode(std::vector<double> channel_data, double sigma_2, cv::Mat pchk, int iAttempt)
{
	wxString	strRetVal = wxEmptyString;
	int			iIteration = 0;
	Initialization(channel_data, 0.5);

	do
	{
		strRetVal.Clear();
		CheckNodeUpdate();
		VariableNodeUpdate();
		iIteration++;

	} while (!Decision(pchk, strRetVal ) && (iIteration<iAttempt) );

	return strRetVal;
}