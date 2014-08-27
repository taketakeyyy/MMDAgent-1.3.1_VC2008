// Thaks : http://www.codeproject.com/Articles/394975/How-To-Use-Kinect-Face-Tracking-SDK
#pragma once 
 
// NuiApi.hの前にWindows.hをインクルードする
#include <Windows.h>
#include <NuiApi.h>
#include <FaceTrackLib.h>
 
#include <iostream>
#include <sstream>
#include <vector>
 
 
#define ERROR_CHECK( ret )  \
    if ( ret != S_OK ) {    \
    std::stringstream ss;	\
    ss << "failed " #ret " " << std::hex << ret << std::endl;			\
    throw std::runtime_error( ss.str().c_str() );			\
    }
 
const NUI_IMAGE_RESOLUTION CAMERA_RESOLUTION = NUI_IMAGE_RESOLUTION_640x480;
 
class KinectSensor
{
private:
 
    INuiSensor* kinect;
    HANDLE imageStreamHandle;
    HANDLE depthStreamHandle;
    HANDLE streamEvent;
 
    DWORD width;
    DWORD height;
 
    IFTFaceTracker* pFT;                // 顔追跡する人
    FT_CAMERA_CONFIG videoCameraConfig; // RGBカメラの設定
    FT_CAMERA_CONFIG depthCameraConfig; // 距離カメラの設定
 
    IFTResult* pFTResult;               // 顔追跡結果
    IFTModel*  pFTModel;                // 検出された顔の3Dモデル
 
    IFTImage* pColorFrame;              // 顔追跡用のRGBデータ
    IFTImage* pDepthFrame;              // 顔追跡用の距離データ
 
    FT_SENSOR_DATA sensorData;          // Kinectセンサーデータ
 
    std::vector<unsigned char> colorCameraFrameBuffer;
    std::vector<unsigned char> depthCameraFrameBuffer;
 
    bool isFaceTracked;
    RECT faceRect;
 
public:
 
 
    KinectSensor()
        : pFT( 0 )
        , pFTResult( 0 )
        , isFaceTracked( false )
    {
    }
 
    ~KinectSensor()
    {
        // 終了処理
        if ( kinect != 0 ) {
            kinect->NuiShutdown();
            kinect->Release();
 
            pFTResult->Release();
            pColorFrame->Release();
            pDepthFrame->Release();
            pFT->Release();
        }
    }
 
    void initialize()
    {
        createInstance();
 
        // Kinectの設定を初期化する
        ERROR_CHECK( kinect->NuiInitialize(
            NUI_INITIALIZE_FLAG_USES_COLOR |
            NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX |
            NUI_INITIALIZE_FLAG_USES_SKELETON ) );
 
        // RGBカメラを初期化する
        ERROR_CHECK( kinect->NuiImageStreamOpen( NUI_IMAGE_TYPE_COLOR, CAMERA_RESOLUTION,
            0, 2, 0, &imageStreamHandle ) );
 
        // 距離カメラを初期化する
        ERROR_CHECK( kinect->NuiImageStreamOpen( NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX, CAMERA_RESOLUTION,
            0, 2, 0, &depthStreamHandle ) );
 
        // Nearモード
        ERROR_CHECK( kinect->NuiImageStreamSetImageFrameFlags(
            depthStreamHandle, NUI_IMAGE_STREAM_FLAG_ENABLE_NEAR_MODE ) );
 
        // スケルトンを初期化する
        ERROR_CHECK( kinect->NuiSkeletonTrackingEnable( 0,
            NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE | 
            NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT ) );
 
        // フレーム更新イベントのハンドルを作成する
        streamEvent = ::CreateEvent( 0, TRUE, FALSE, 0 );
        ERROR_CHECK( kinect->NuiSetFrameEndEvent( streamEvent, 0 ) );
 
        // 指定した解像度の、画面サイズを取得する
        ::NuiImageResolutionToSize(CAMERA_RESOLUTION, width, height );
 
        // 顔認識の初期化
        initializeFaceTracker();
    }
 
    void initializeFaceTracker()
    {
        // FaceTrackerのインスタンスを生成する
        pFT = ::FTCreateFaceTracker();
        if( !pFT ) {
            throw std::runtime_error( "ERROR:FTCreateFaceTracker" );
        }
 
        // RGBカメラおよび距離カメラの設定を行う
        videoCameraConfig.Width = width;
        videoCameraConfig.Height = height;
        videoCameraConfig.FocalLength =
                    NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS * (width / 640.0f);
 
        depthCameraConfig.Width = width;
        depthCameraConfig.Height = height;
        depthCameraConfig.FocalLength = 
                    NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS * (width / 320.0f);
 
        // FaceTrackerを初期化する
        HRESULT hr = pFT->Initialize( &videoCameraConfig, &depthCameraConfig, 0, 0) ;
        if( FAILED(hr) ) {
            throw std::runtime_error( "ERROR:Initialize" );
        }
 
        // FaceTrackerの結果を格納先を生成する
        hr = pFT->CreateFTResult( &pFTResult );
        if(FAILED(hr)) {
            throw std::runtime_error( "ERROR:CreateFTResult" );
        }
 
        // FaceTrackerで利用するRGBおよび距離データのインスタンスを生成する
        pColorFrame = FTCreateImage();
        pDepthFrame = FTCreateImage();
//        if(!pDepthFrame ) {
		if( !pColorFrame || !pDepthFrame ) {
            throw std::runtime_error( "ERROR:FTCreateImage" );
        }
 
        // RGBおよび距離データのバッファサイズを設定する
        // RGBは1pixelあたり4バイト。距離は1pixelあたり2バイト
        colorCameraFrameBuffer.resize( width*4 * height );
        depthCameraFrameBuffer.resize( width*2 * height );
 
        // フレームデータにバッファを設定する
        // You can also use Allocate() method in which case IFTImage interfaces own their memory.
        // In this case use CopyTo() method to copy buffers
        // CopyToでもOK?
       pColorFrame->Attach(width, height, &colorCameraFrameBuffer[0], FTIMAGEFORMAT_UINT8_B8G8R8X8, width*4);
        pDepthFrame->Attach(width, height, &depthCameraFrameBuffer[0], FTIMAGEFORMAT_UINT16_D13P3, width*2);
 
        // センサーデータを作成する
        sensorData.pVideoFrame = pColorFrame;
        sensorData.pDepthFrame = pDepthFrame;
        sensorData.ZoomFactor = 1.0f;			// Not used must be 1.0
        sensorData.ViewOffset.x = 0;			// Not used must be (0,0)
        sensorData.ViewOffset.y = 0;			// Not used must be (0,0)
    }
 
    void update()
    {
        // データの更新を待つ
//        DWORD ret = ::WaitForSingleObject( streamEvent, INFINITE );
//        ::ResetEvent( streamEvent );
        copyStreamData( imageStreamHandle, colorCameraFrameBuffer );
        copyStreamData( depthStreamHandle, depthCameraFrameBuffer );
        faceTracking();
    }
 
    const std::vector<unsigned char>& getColorFrameData() const
    {
        return colorCameraFrameBuffer;
    }
 
    const std::vector<unsigned char>& getDepthFrameData() const
    {
        return depthCameraFrameBuffer;
    }
 
    bool IsFaceTracked() const
    {
        return isFaceTracked;
    }
 
    const RECT& FaceRect()
    {
        return faceRect;
    }
 
    DWORD getWidth() const
    {
        return width;
    }
 
    DWORD getHeight() const
    {
        return height;
    }
 
private:
 
    void createInstance()
    {
        // 接続されているKinectの数を取得する
        int count = 0;
        ERROR_CHECK( ::NuiGetSensorCount( &count ) );
        if ( count == 0 ) {
            throw std::runtime_error( "Kinect を接続してください" );
        }
 
        // 最初のKinectのインスタンスを作成する
        ERROR_CHECK( ::NuiCreateSensorByIndex( 0, &kinect ) );
 
        // Kinectの状態を取得する
        HRESULT status = kinect->NuiStatus();
        if ( status != S_OK ) {
            throw std::runtime_error( "Kinect が利用可能ではありません" );
        }
    }
 
    void copyStreamData( HANDLE streamHandle, std::vector<unsigned char>& buffer )
    {
        // RGBカメラのフレームデータを取得する
        NUI_IMAGE_FRAME frame = { 0 };
        ERROR_CHECK( kinect->NuiImageStreamGetNextFrame( streamHandle, INFINITE, &frame ) );
 
        // 画像データを取得する
        NUI_LOCKED_RECT data;
        frame.pFrameTexture->LockRect( 0, &data, 0, 0 );
 
        // データをコピーする
        if ( data.size != buffer.size() ) {
            buffer.resize( data.size );
        }
 
        std::copy( data.pBits, data.pBits + data.size, buffer.begin() );
 
        // フレームデータを解放する11111
        ERROR_CHECK( kinect->NuiImageStreamReleaseFrame( streamHandle, &frame ) );
    }
 
    std::vector<unsigned char> copyStreamData( HANDLE streamHandle )
    {
        std::vector<unsigned char> buffer;
        copyStreamData( streamHandle, buffer );
        return buffer;
    }
 
private:
 
    // 顔追跡(内部用)
    HRESULT faceTracking_p()
    {
        // 追跡中、未追跡によって処理が変わる
        if(!isFaceTracked) {
            // FaceTrakingを開始する
            return pFT->StartTracking(&sensorData, NULL, NULL, pFTResult);
        }
        else {
            // FaceTrakingを継続する
            return pFT->ContinueTracking(&sensorData, NULL, pFTResult);
        }
    }
 
private:
 
    FT_VECTOR2D* points;
    UINT pointCount;
 
public:
 
    FT_VECTOR2D* get2DPoints()const
    {
        return points;
    }
 
    UINT get2DPointCount() const
    {
        return pointCount;
    }
 
    IFTModel* getFaceModel() const
    {
        return pFTModel;
    }
 
    IFTResult* getResult() const
    {
        return pFTResult;
    }
 
    // 顔追跡(公開用)
    void faceTracking()
    {
        // 顔追跡
        HRESULT hr = faceTracking_p();
        // 顔を見つけたので、追跡状態へ遷移
        if(SUCCEEDED(hr) && SUCCEEDED(pFTResult->GetStatus())) {
//        if(SUCCEEDED(pFTResult->GetStatus())) {

			//            ::OutputDebugString( L"FaceTracking Success\n" );
            isFaceTracked = true;
 
            // 顔の領域を取得する
            pFTResult->GetFaceRect( &faceRect );
 /*
            // 2Dの座標を取得する
            points = 0;
            pointCount = 0;
            hr = pFTResult->Get2DShapePoints( &points, &pointCount );
            if( FAILED(hr) ) {
                ::OutputDebugString( L"Get2DShapePoints Failed\n" );
            }
 */
            pFT->GetFaceModel( &pFTModel );
			
        }
        // 顔を見失ったので、未追跡状態のまま
        else {
//            ::OutputDebugString( L"FaceTracking failed\n" );
            isFaceTracked = false;
        }
    }
};
