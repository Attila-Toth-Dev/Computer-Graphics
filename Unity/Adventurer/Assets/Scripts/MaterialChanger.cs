using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using UnityEngine.UI;

public class MaterialChanger : MonoBehaviour
{
    [SerializeField] private Material playerMaterial;

    [SerializeField] private Slider sliderR;
    [SerializeField] private Slider sliderG;
    [SerializeField] private Slider sliderB;
    [SerializeField] private Slider sliderA;

    private int counter;

    private Color customColor;

    private void Start()
    {
        counter = 0;
    }

    private void Update()
    {
        ChangeColour();
    }

    /*public void ChangeMaterial(int _value)
    {
        counter += _value;

        Debug.Log(counter);

        if (counter <= 0)
            counter = materials.Length - 1;

        if (counter >= materials.Length)
            counter = 0;

        playerMaterial.color = materials[counter].color;
    }*/

    public void ChangeSmoothness(float _intensity)
    {
        playerMaterial.SetFloat("_Smoothness", _intensity);
    }

    public void ChangeColour()
    {
        customColor = new Color(sliderR.value, sliderG.value, sliderB.value, sliderA.value);
        playerMaterial.color = customColor;
        //playerMaterial.SetColor("_Color", customColor);
    }
}
