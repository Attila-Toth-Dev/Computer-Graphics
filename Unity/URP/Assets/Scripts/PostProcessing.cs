using UnityEngine;

public class PostProcessing : MonoBehaviour
{
    public Material mat;

    private void Start()
    {
        Camera camera = GetComponent<Camera>();
        camera.depthTextureMode = DepthTextureMode.Depth;
    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        mat.SetFloat("_DeltaX", 1.0f / (float)source.width);
        mat.SetFloat("_DeltaY", 1.0f / (float)source.height);

        Graphics.Blit(source, destination, mat);
    }
}
