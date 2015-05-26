
void Engine::Sprite::SetSpriteSheetProperties(Engine::SpriteSheetProperties i_spriteSheetProperties)
{
	m_sheetProperties = i_spriteSheetProperties;
}

Engine::SpriteSheetProperties Engine::Sprite::GetSpriteSheetProperties()
{
	return m_sheetProperties;
}


void Engine::Sprite::SetCurrentSpriteSheetElement(unsigned int i_element)
{
	m_sheetProperties.m_currentElement = i_element;
}

void Engine::Sprite::SetNativeResolution(Engine::NativeResolution i_resolution)
{
	m_nativeResolution = i_resolution;
}

Engine::NativeResolution Engine::Sprite::GetNativeResolution()
{
	return m_nativeResolution;
}

void Engine::Sprite::AdjustDimensionsForResolution(Quad* i_quad)
{
	i_quad->setWidth(i_quad->getWidth()*m_nativeResolution.m_width / eae6320::UserSettings::GetWidth());
	i_quad->setHeight(i_quad->getHeight()*m_nativeResolution.m_height / eae6320::UserSettings::GetHeight());
}
