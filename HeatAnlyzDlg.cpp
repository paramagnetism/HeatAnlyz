
// HeatAnlyzDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "HeatAnlyz.h"
#include "HeatAnlyzDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHeatAnlyzDlg 对话框



CHeatAnlyzDlg::CHeatAnlyzDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HEATANLYZ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHeatAnlyzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BMP_FILE, m_staticBMPFilePath);
	DDX_Control(pDX, IDC_STATIC_TXT_FILE, m_staticTXTFilePath);
}

BEGIN_MESSAGE_MAP(CHeatAnlyzDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_BMP, &CHeatAnlyzDlg::OnBnClickedButtonLoadBmp)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TXT, &CHeatAnlyzDlg::OnBnClickedButtonLoadTxt)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_RST, &CHeatAnlyzDlg::OnBnClickedButtonSaveRst)
	ON_BN_CLICKED(IDC_BUTTON_CONFIRM, &CHeatAnlyzDlg::OnBnClickedButtonConfirm)
END_MESSAGE_MAP()


// CHeatAnlyzDlg 消息处理程序

BOOL CHeatAnlyzDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHeatAnlyzDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHeatAnlyzDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHeatAnlyzDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHeatAnlyzDlg::OnBnClickedButtonLoadBmp()
{
	CString filter = _T("Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filter, this);

	if (dlg.DoModal() == IDOK)
	{
		m_strBMPFilePath = dlg.GetPathName();
		m_staticBMPFilePath.SetWindowText(m_strBMPFilePath);

		// Load BMP image using OpenCV
		std:: string filename = CT2A(m_strBMPFilePath);
		BMPimage = cv::imread(filename, cv::IMREAD_GRAYSCALE);
		if (!BMPimage.empty())
		{
			// Crop pixels off the right side of the BMP image
			cv::Rect croppedRegion(0, 0, BMPimage.cols - 51, BMPimage.rows);
			BMPimage = BMPimage(croppedRegion);

			// Apply threshold to obtain binary image
			cv::Mat binaryImage;
			cv::threshold(BMPimage, binaryImage, 250, 255, cv::THRESH_BINARY);

			// Show binary image using OpenCV's imshow
			cv::imshow("Binary Image", BMPimage);
			cv::waitKey(0); // Wait for a key press
		}
	}
}


void CHeatAnlyzDlg::OnBnClickedButtonLoadTxt()
{
	CString filter = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, filter, this);

	if (dlg.DoModal() == IDOK)
	{
		m_strTXTFilePath = dlg.GetPathName();
		m_staticTXTFilePath.SetWindowText(m_strTXTFilePath);

		// Read the TXT file starting from the 19th line
		std:: string filename = CT2A(m_strTXTFilePath);
		std:: ifstream file(filename);
		std:: string line;
		for (int i = 0; i < 18; ++i)
			std::getline(file, line);

		// Parse the data into a 2D matrix
		while (std::getline(file, line))
		{
			std::istringstream iss(line);
			std::vector<double> row;
			std::string token;
			while (std::getline(iss, token, '\t'))
			{
				// Replace ',' with '.'
				std::replace(token.begin(), token.end(), ',', '.');
				row.push_back(std::stod(token));
			}
			data.push_back(row);
		}

		// Normalize the matrix to 0-255 scale
		double minVal = DBL_MAX;
		double maxVal = -DBL_MAX;
		for (const auto& row : data)
		{
			for (double val : row)
			{
				minVal = std::min(minVal, val);
				maxVal = std::max(maxVal, val);
			}
		}

		cv::Mat image(data.size(), data[0].size(), CV_8UC1);
		for (size_t i = 0; i < data.size(); ++i)
		{
			for (size_t j = 0; j < data[i].size(); ++j)
			{
				double normalized = 255 * (data[i][j] - minVal) / (maxVal - minVal);
				image.at<uchar>(i, j) = static_cast<uchar>(normalized);
			}
		}

		TXTimage = image;
		// Display the image using OpenCV
		cv::imshow("Image from TXT", image);
		cv::waitKey(0); // Wait for a key press
	}
}


void CHeatAnlyzDlg::OnBnClickedButtonSaveRst()
{
	if (BMPimage.empty() || TXTimage.empty())
	{
		AfxMessageBox(_T("Please load both BMP and TXT files first."));
		return;
	}

	cv::Ptr<cv::Feature2D> sift = cv::SIFT::create();

	// Detect keypoints and compute descriptors for both images
	std::vector<cv::KeyPoint> keypointsBinary, keypointsText;
	cv::Mat descriptorsBinary, descriptorsText;
	sift->detectAndCompute(BMPimage, cv::noArray(), keypointsBinary, descriptorsBinary);
	sift->detectAndCompute(TXTimage, cv::noArray(), keypointsText, descriptorsText);

	// Match descriptors using FLANN matcher
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
	std::vector<std::vector<cv::DMatch>> knnMatches;
	matcher->knnMatch(descriptorsBinary, descriptorsText, knnMatches, 2);

	// Filter good matches using Lowe's ratio test
	std::vector<cv::DMatch> goodMatches;

	const float ratioThreshold = 0.84f;
	for (size_t i = 0; i < knnMatches.size(); ++i)
	{
		if (knnMatches[i][0].distance < ratioThreshold * knnMatches[i][1].distance)
		{
			goodMatches.push_back(knnMatches[i][0]);
		}
	}

	// Extract corresponding keypoints
	std::vector<cv::Point2f> pointsBinary, pointsText;
	for (const auto& match : goodMatches)
	{
		pointsBinary.push_back(keypointsBinary[match.queryIdx].pt);
		pointsText.push_back(keypointsText[match.trainIdx].pt);
	}

	// Find homography matrix
	homography = cv::findHomography(pointsBinary, pointsText, cv::RANSAC);

	// Warp binary image onto the text image
	cv::Mat warpedBinaryImage;
	cv::warpPerspective(BMPimage, warpedBinaryImage, homography, TXTimage.size());

	// Blend images
	cv::Mat mergedImage;
	cv::addWeighted(TXTimage, 0.5, warpedBinaryImage, 0.5, 0, mergedImage);

	// Show and save the merged image
	cv::imshow("Merged Image", mergedImage);
	// cv::imwrite("files/merged_image.png", mergedImage);
	cv::waitKey(0);

}


void CHeatAnlyzDlg::OnBnClickedButtonConfirm()
{
	if (BMPimage.empty() || TXTimage.empty())
	{
		AfxMessageBox(_T("Please load both BMP and TXT files first."));
		return;
	}
	// Apply a threshold of 250 on BMP image to obtain highlight region
	cv::Mat highlightRegion;
	cv::threshold(BMPimage, highlightRegion, 250, 255, cv::THRESH_BINARY);

	// Transfer all coordinates of highlight region on BMP image to TXT image with the homography matrix
	cv::Mat warpedHighlightRegion;
	cv::warpPerspective(highlightRegion, warpedHighlightRegion, homography, TXTimage.size());

	// Transfer the merged image from gray to color
	cv::Mat mergedImageCopy;
	cv::cvtColor(TXTimage, mergedImageCopy, cv::COLOR_GRAY2BGR);
	
	// Iterate over the highlight region on TXT image
	CString filter = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog saveDlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT, filter, this);
	

	std::ofstream outputFile("files/overexposed_pixels.txt");
	if (saveDlg.DoModal() == IDOK) {
		CString saveFilePath = saveDlg.GetPathName();
		std::ofstream outputFile(CT2A(saveFilePath));
	}

	for (int y = 0; y < warpedHighlightRegion.rows; ++y)
	{
		for (int x = 0; x < warpedHighlightRegion.cols; ++x)
		{
			if (warpedHighlightRegion.at<uchar>(y, x) > 0)
			{
				// Check if the pixel value is the maximum among pixels around it
				bool isMax = true;
				int size = 1;
				for (int dy = -size; dy <= size; dy++)
				{
					for (int dx = -size; dx <= size; dx++)
					{
						int yy = y + dy;
						int xx = x + dx;
						if (yy >= 0 && yy < TXTimage.rows && xx >= 0 && xx < TXTimage.cols)
						{
							if (data[yy][xx] > data[y][x])
							{
								isMax = false;
								break;
							}
						}
					}
					if (!isMax)
						break;
				}

				if (isMax)
				{
					// Save the pixel coordinates on BMP image and pixel value on TXT matrix to a new .txt file
					outputFile << y << ", " << x << ", " << data[y][x] << std::endl;
					
					// Mark the pixel green on the color merged image
					cv::circle(mergedImageCopy, Point2i(x, y), 1, cv::Scalar(0, 255, 0), -1);
				}
			}
		}
	}
	outputFile.close();
	// Show the color merged image
	cv::imshow("Color Merged Image with Overexposed Pixels", mergedImageCopy);
	cv::waitKey(0); // Wait for a key press
}
