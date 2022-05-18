#pragma once

#ifndef BB2D_VERSION_H
#define BB2D_VERSION_H

namespace bb2d {
	namespace debug {
#ifndef BB2D_VERSION
		#define BB2D_VERSION std::make_pair(1, 0)
#endif
	}
}

#endif // !BB2D_VERSION_H
