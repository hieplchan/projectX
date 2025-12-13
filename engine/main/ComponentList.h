#include <unordered_map>

#include <Metadata.h>

#include <components/Transform.h>
#include <components/Camera.h>
#include <components/QuadRenderer.h>
#include <components/RotatorComp.h>
#include <components/SpriteRenderer.h>

#define COMPONENT_LIST(X) \
    X(Transform) \
    X(Camera) \
    X(QuadRenderer) \
    X(RotatorComp) \
    X(SpriteRenderer)
