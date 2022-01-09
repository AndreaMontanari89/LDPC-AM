#ifndef __TANNERGRAPH_H__
#define __TANNERGRAPH_H__
#pragma once


class CTannerNode
{
public:
	CTannerNode() {};
	~CTannerNode() {};

	virtual double Compute();

private:

	// dati relativi alla logica
	//
	std::list<CTannerNode*>		m_pLinks;
	int							m_iPosition;

	// dati relativi al plot
	cv::Point2f					m_PtCenter;
	cv::Scalar					m_Clr;
};


class CTannerGraph
{
	class TCheckNode : public CTannerNode
	{

	};
	class TVirtualNode : public CTannerNode
	{

	};


	void	Draw(cv::Mat mCanvas);

private:
	std::list<TCheckNode*>		m_pCheckNodesList;
	std::list<TVirtualNode*>	m_pVirtualNodesList;
};

#endif