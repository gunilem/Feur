#pragma once

namespace Feur {

	class Layer
	{
	public:
		virtual void OnUpdate() = 0;
		virtual const std::string& GetName() const = 0;
	};

}

