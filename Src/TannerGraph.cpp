#include "stdafx.h"
#include "TannerGraph.h"

CTannerGraph::CTannerGraph(int N, int M)
{
	m_iN = N;
	m_iM = M;
	// istanzio tutti i nodi
	for (int i = 0; i < N; i++)
		m_pVariableNodesList.push_back(new TVariableNode(i+1));

	for (int i = 0; i < M; i++)
		m_pCheckNodesList.push_back(new TCheckNode(i+1));
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
		(*itV)->AddLink((*itC));
		(*itC)->AddLink((*itV));

	} while(0);
}

void CTannerGraph::Initialization( std::vector<double> channel_data, double sigma_2 )
{
	do
	{
		if (channel_data.size() != m_iN)
			break;

		for (int i = 0; i < m_iN; i++)
			m_dLU.push_back(2 * channel_data[i] / sigma_2);

		for (int c = 0; c < m_iM; c++)
			for (int k = 0; k < m_pCheckNodesList[c]->GetNumLinks(); k++)
			{
				CTannerNode* pLink = m_pCheckNodesList[c]->GetLink(k); // Variable node collegato				
				int			 iDstID = m_pCheckNodesList[c]->GetNodeID();

				m_pCheckNodesList[c]->SetMetric(m_dLU[iDstID], 0.0, k);
			}

		for (int v = 0; v < m_iN; v++)
			for (int k = 0; k < m_pVariableNodesList[v]->GetNumLinks(); k++)
			{
				CTannerNode* pLink = m_pCheckNodesList[v]->GetLink(k); // Check node collegato				
				int			 iDstID = m_pCheckNodesList[v]->GetNodeID();
				m_pVariableNodesList[v]->SetMetric(m_dLU[v], 0.0, k);
			}
				


	} while (0);
}

void CTannerGraph::CheckNodeUpdate()
{
	
	double dPrAccum = 0.0;

	for (int c = 0; c < m_iM; c++)
	{
		for (int v = 0; v < m_pCheckNodesList[c]->GetNumLinks(); v++)
		{
			double dAccum = 0.0;
			CTannerNode* pLink = m_pCheckNodesList[c]->GetLink(v); // Variable node collegato				
			int			 iSrcID = m_pCheckNodesList[c]->GetNodeID();

			for (int n = 0; n < m_pCheckNodesList[c]->GetNumLinks(); n++)
			{
				int	 iDstID = m_pCheckNodesList[c]->GetNodeID();
				if (iDstID != iSrcID)
				{
					std::pair<double, double> metrics = pLink->GetMetric(n);
					dAccum *= tanh(metrics.first / 2.0);
				}

			}
			m_pCheckNodesList[c]->SetMetric(DBL_MIN, 2.0*atanh(dAccum), v);
		}		
	}
		
}

void CTannerGraph::VariableNodeUpdate()
{
	for (int v = 0; v < m_iN; v++)
	{
		for (int c = 0; c < m_pVariableNodesList[c]->GetNumLinks(); c++)
		{
			double dAccum = 0.0;
			CTannerNode* pLink = m_pCheckNodesList[c]->GetLink(c); // Check node collegato				
			int			 iSrcID = m_pCheckNodesList[c]->GetNodeID();

			for (int m = 0; m < m_pVariableNodesList[c]->GetNumLinks(); m++)
			{
				int	 iDstID = m_pCheckNodesList[c]->GetNodeID();
				if (iDstID != iSrcID)
				{
					std::pair<double, double> metrics = pLink->GetMetric(m);
					dAccum += metrics.second;
				}

			}
			m_pVariableNodesList[v]->SetMetric(m_dLU[v]+dAccum, DBL_MIN, c);
		}
	}
}

bool CTannerGraph::Decision()
{
	bool bRetVal = false;
	std::vector<double> result;
	for (int v = 0; v < m_iN; v++)
	{
		double dAccum = 0.0;
		for (int c = 0; c < m_pVariableNodesList[c]->GetNumLinks(); c++)
		{
			CTannerNode* pLink = m_pCheckNodesList[c]->GetLink(c); // Check node collegato				

			std::pair<double, double> metrics = pLink->GetMetric(c);
			dAccum += metrics.second;
		}

		result.push_back(m_dLU[v] + dAccum);
	}

	cv::Mat word = cv::Mat::zeros(1, m_iN, CV_8UC1);
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i] >= 0)
			word.at<uchar>(1, i) = 1;
	}

	return bRetVal;
}