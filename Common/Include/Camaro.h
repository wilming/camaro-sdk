#pragma once

#include "IExtensionAccess.h"
#include "ICameraControl.h"
#include "IDeviceControl.h"
#include "CameraSoloBase.h"
#include "ILowlevelControl.h"
#include "ExtensionAccessAdapter.h"


namespace TopGear
{
	class Camaro
		: public CameraSoloBase,
		public TopGear::ICameraControl,
		public IDeviceControl,
		public ILowlevelControl
	{
	public:
		virtual int GetOptimizedFormatIndex(VideoFormat& format, const char* fourcc="") override;
		virtual int GetMatchedFormatIndex(const VideoFormat& format) const override;
		virtual const std::vector<VideoFormat>& GetAllFormats() const override;

		virtual const VideoFormat &GetCurrentFormat() const override;
		virtual bool SetCurrentFormat(uint32_t formatIndex) override;	

		virtual bool StartStream() override;
		virtual bool StopStream() override;

		virtual bool SetControl(std::string name, IPropertyData &val) override;
		virtual bool SetControl(std::string name, IPropertyData &&val) override;
		virtual bool GetControl(std::string name, IPropertyData &val) override;

		//advanced device controls (on EP0)
		virtual int SetRegisters(uint16_t regaddr[], uint16_t regval[], int num) override;
		virtual int GetRegisters(uint16_t regaddr[], uint16_t regval[], int num) override;
		//single register
		virtual int SetRegister(uint16_t regaddr, uint16_t regval) override;
		virtual int GetRegister(uint16_t regaddr, uint16_t &regval) override;
	protected:
        virtual void PostProcess(std::vector<IVideoFramePtr> &frames) override;
        ExtensionAccessAdapter extensionAdapter;
		std::vector<VideoFormat> formats;
		int currentFormatIndex = -1;
	private:
		static const int EMBEDDED_LINES = 2;
		void ObtainExtendedLines();
		int header = 0;
		int footer = 0;
		const RegisterMap * registerMap = nullptr;
		std::string sensorInfo;
	public:
		virtual int Flip(bool vertical, bool horizontal) override;
        virtual int GetExposure(bool &ae, float &ev) override;
        virtual int SetExposure(bool ae, float ev=1.0f) override;
        virtual int GetShutter(uint32_t &val) override;
        virtual int SetShutter(uint32_t val) override;
        virtual int GetGain(float &gainR, float &gainG, float &gainB) override;
        virtual int SetGain(float gainR, float gainG, float gainB) override;
        virtual int GetIris(float &ratio) override { ratio=0; return -1; }
        virtual int SetIris(float ratio) override { (void)ratio; return -1; }

		Camaro(std::shared_ptr<IVideoStream> &vs, 
			   std::shared_ptr<IExtensionAccess> &ex,
			CameraProfile &con = CameraProfile::NullObject());
		virtual ~Camaro();
	};
}

