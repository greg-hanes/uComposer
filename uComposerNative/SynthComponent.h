#pragma once


namespace uComposer
{
	class BaseEffect;

	class SynthComponent
	{
	public:
		virtual void OnRegister(BaseEffect *) = 0;
		virtual void OnUnregister(BaseEffect *) = 0;
	private:
	};
}