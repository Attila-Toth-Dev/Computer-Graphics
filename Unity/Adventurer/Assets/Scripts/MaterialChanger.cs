using System.Linq;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public class MaterialChanger : MonoBehaviour
{
    [SerializeField] private Material playerMaterial;
    [SerializeField] private GameObject playerModelSurface;

    [SerializeField] private Material rimMaterial;
    [SerializeField] private GameObject[] barrelModels;
    [SerializeField] private ParticleSystem[] particlesSystems;

    [SerializeField] private Slider smoothnessSlider;
    [SerializeField] private Slider intensitySlider;
    [SerializeField] private Slider metallicSlider;

    [SerializeField] private Slider sliderR;
    [SerializeField] private Slider sliderG;
    [SerializeField] private Slider sliderB;

    private bool _isShown = true;

    private void Awake()
    {
        playerMaterial = playerModelSurface.GetComponent<Renderer>().material;
    }

    private void Update()
    {
        ChangeColour();
        ChangeSmoothness();
        ChangeIntensity();
        ChangeMetallic();
    }

    public void ChangeIntensity()
    {
        for(int i = 0; i < barrelModels.Length; i++)
        {
            rimMaterial.SetFloat("_RimPower", intensitySlider.value);
        }
    }

    public void ChangeSmoothness()
    {
        playerMaterial.SetFloat("_Smoothness", smoothnessSlider.value);
    }

    public void ChangeMetallic()
    {
        playerMaterial.SetFloat("_Metallic", metallicSlider.value);
    }

    public void ChangeColour()
    {
        playerMaterial.SetColor("_Colour", new Vector4(sliderR.value, sliderG.value, sliderB.value, 1));

        for (int i = 0; i < barrelModels.Length; i++)
        {
            rimMaterial.SetColor("_RimColour", new Vector4(sliderR.value, sliderG.value, sliderB.value, 1));
        }
    }

    public void ToggleParticles()
    {
        _isShown = !_isShown;

        for (int i = 0; i < particlesSystems.Length; i++)
        {
            particlesSystems[i].gameObject.SetActive(_isShown);
        }
    }
}
