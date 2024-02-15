## 喷嘴口计算程序
	opencv version = 4.6.0
	MFC 2019 
	VS 2019

由于是SIFT + KNN RANSAC做的特征点匹配，匹配出来可能对不准。
匹配特征点是预览效果，如果结果满意可以保存喷嘴口点的温度，不满意可以再点一次匹配特征点。
想改进匹配效果的话建议把 txt 的图像也像BMP的裁一下，但需要多一些txt的图参考位置。    ￥100

内容主要在 HeatAnlyzDlg.cpp，界面设置调整在资源视图。

# HeatAnlyzDlg.cpp 可调参数所在行：

			// 裁剪BMP图像最右边的标尺，设置默认为51个像素。
	187   	cv::Rect croppedRegion(0, 0, BMPimage.cols - 51, BMPimage.rows);

			// 特征点匹配的分数阈值，默认用匹配度大于84%的点
			// 阈值高可能导致匹配出的结果不稳定，阈值低则可能算出喷嘴点数目变少
	288		const float ratioThreshold = 0.84f;

			// 点比其周围多少点的温度高则保存下来。为1为9x9范围，2则为25x25范围。
	362		int size = 1;