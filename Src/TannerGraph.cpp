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
	__CleanUpNode();
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

void CTannerGraph::Initialization( std::vector<double> channel_data, int iChType, double ch_par)
{
	do
	{
		if (channel_data.size() != m_iN)
			break;
		m_dLU.clear();

		for (int i = 0; i < m_iN; i++)
			if (iChType == 1)
				m_dLU.push_back(2 * channel_data[i] / ch_par);
			else
				if(channel_data[i] == 0.0 )
					m_dLU.push_back(-log10((1.0- ch_par)/ ch_par));
				else
					m_dLU.push_back(log10((1.0 - ch_par) / ch_par));
				

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
					
					if ((metrics == -INFINITY) && (dAccum == INFINITY) ||
						(metrics == INFINITY) && (dAccum == -INFINITY))
						dAccum = 0.0;
					else
						dAccum += metrics;
				} 

			}

			double dMetric;
			if ((m_dLU[v] == -INFINITY) && (dAccum == INFINITY) ||
				(m_dLU[v] == INFINITY) && (dAccum == -INFINITY))
				dMetric = 0.0;
			else
				dMetric = m_dLU[v] + dAccum;

			m_pVariableNodesList[v]->SetMetric(iLinkIdx, dMetric);
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

	// controllo che u*Ht = 0 ma anche che u  non sia la parola nulla
	if (cv::countNonZero(res) == 0 && cv::countNonZero(word) != 0)
		bRetVal = true;


	return bRetVal;
}

wxString CTannerGraph::Decode(std::vector<double> channel_data, int iChType, double ch_par, cv::Mat pchk, int iAttempt, int* piAttemptUsed)
{
	wxString	strRetVal = wxEmptyString;
	int			iIteration = 0;
	Initialization(channel_data, iChType, ch_par);

	do
	{
		strRetVal.Clear();
		CheckNodeUpdate();
		VariableNodeUpdate();
		iIteration++;

	} while (!Decision(pchk, strRetVal ) && (iIteration<iAttempt) );

	if (piAttemptUsed != NULL)
		*piAttemptUsed = iIteration;

	return strRetVal;
}

void CTannerGraph::Draw()
{
	int iWidthPadPx = 30;		// Pixel di padding in larghezza
	int iHeigttPadPx = 20;		// Pixel di padding in altezza
	int iCircleRadius = 20;		// Raggio cerchi variable nodes
	int iSquareLenght = 40;		// Lato quadrati check nodes
	int iVNSpace = 30;			// Spazio tra i variable nodes
	int iCNSpace = 30;			// Spazio tra i check nodes
	int iVNCNSpace = 400;		// Spazio in altezza tra le due file di nodi

	cv::Scalar CNCol = cv::Scalar(168, 235, 188);
	cv::Scalar VNCol = cv::Scalar(227, 189, 255);

	int iWidthGrph = (m_iN * (iCircleRadius * 2)) + ((m_iN - 1) * iVNSpace) + (2 * iWidthPadPx);
	int HeihgtGrph = (iCircleRadius * 2) + iVNCNSpace + iSquareLenght + (2 * iHeigttPadPx);

	cv::Mat mCanvas = cv::Mat(HeihgtGrph, iWidthGrph, CV_8UC3);
	mCanvas = cv::Scalar(255, 255, 255);

	std::vector<cv::Point> VNAttachPoints;  // punti di attacco per ogni nodo per partenza e arrivo link
	std::vector<cv::Point> CNAttachPoints;

	// disegno i variable nodes
	for (int v = 0; v < m_iN; v++)
	{
		int iCX = iWidthPadPx + (((v + 1) * iCircleRadius) + (v * (iVNSpace + iCircleRadius)));
		int iCY = iHeigttPadPx + iCircleRadius;
		cv::circle(mCanvas, cv::Point(iCX, iCY), iCircleRadius, CNCol, -1);
		cv::putText(mCanvas, wxString::Format("VN%d", v + 1).ToStdString(), cv::Point(iCX, iCY - iCircleRadius - 2), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 0));
		VNAttachPoints.push_back(cv::Point(iCX, iCY + iCircleRadius));
	}

	//disegno i check node
	int iSpaceCNReq = ((m_iM * iSquareLenght) + ((m_iM - 1) * iCNSpace)); // spazioe che occupano i CN nel graifco mi serve per centrarli
	int iFirstX = (iWidthGrph / 2) - (iSpaceCNReq / 2);
	for (int c = 0; c < m_iM; c++)
	{
		int iX = iFirstX + (c* iSquareLenght) + (c* iCNSpace);
		int iY = mCanvas.rows - iHeigttPadPx - iSquareLenght;
		int iW = iSquareLenght;
		int iH = iSquareLenght;

		int iCX = (iX + (iW / 2));
		int iCY = (iY + (iW / 2));

		cv::rectangle(mCanvas, cv::Rect(iX, iY, iW, iH), VNCol, -1);
		cv::putText(mCanvas, wxString::Format("CN%d", c + 1).ToStdString(), cv::Point(iCX, iCY + iCircleRadius + 12), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 0));
		CNAttachPoints.push_back(cv::Point(iCX, iCY - (iSquareLenght/2)));
	}

	// disegno i link
	cv::RNG rng(12345);
	for (int v = 0; v < m_iN; v++)
	{
		for (int l = 0; l < m_pVariableNodesList[v]->GetNumLinks(); l++)
		{
			int iSrcNode = m_pVariableNodesList[v]->GetNodeID();
			int iDstNode = m_pVariableNodesList[v]->GetLink(l)->GetNodeID();

			cv::line(mCanvas, VNAttachPoints[iSrcNode-1], CNAttachPoints[iDstNode-1], cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)),2);
		}
	}

	
	auto axes = CvPlot::plotImage(mCanvas);
	cv::Mat mat = axes.render(500, 400);
	axes.setMargins(0, 0, 0, 0);
	CvPlot::show("Tanner Graph", axes);

}

void CTannerGraph::__CleanUpNode()
{
	// istanzio tutti i nodi
	for (int i = 0; i < m_iN; i++)
		delete m_pVariableNodesList[i];

	for (int i = 0; i < m_iM; i++)
		delete m_pCheckNodesList[i];
}