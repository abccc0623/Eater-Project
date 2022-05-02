#pragma once
class EnvironmentPass : public RenderPassBase
{
public:
	EnvironmentPass();
	~EnvironmentPass();

public:
	void Create(int width, int height) override;
	void Start(int width, int height) override;

	void OnResize(int width, int height) override;
	void Release() override;

	void ApplyOption() override;
	void SetEnvironmentMapResource(EnvironmentBuffer* resource);

	void RenderUpdate();

private:
	DrawBuffer* m_Box_DB;

	VertexShader* m_SkyBox_VS;
	PixelShader* m_SkyBox_PS;

	RenderTexture* m_OutPut_RT;
	RenderTexture* m_Position_RT;
	
private:
	ID3D11RasterizerState* m_CubeMap_RS;
	ID3D11DepthStencilState* m_CubeMap_DSS;

	std::vector<ID3D11RenderTargetView*> m_RTV_List;
	ID3D11DepthStencilView* m_Defalt_DSV;

private:
	Matrix m_EnvironmentWorld;
};

