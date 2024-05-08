using UnityEngine;

public class MaterialChanger : MonoBehaviour
{
    [SerializeField] private Material playerMaterial;
    [SerializeField] private Material[] materials;

    private int counter;

    private void Start()
    {
        counter = 0;

        playerMaterial.color = materials[0].color;
    }

    public void ChangeMaterial(int _value)
    {
        counter += _value;

        Debug.Log(counter);

        if (counter <= 0)
            counter = materials.Length - 1;

        if (counter >= materials.Length)
            counter = 0;

        playerMaterial.color = materials[counter].color;
    }
}
