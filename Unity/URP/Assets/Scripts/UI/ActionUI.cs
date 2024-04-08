using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class ActionUI : MonoBehaviour
{
    public Action action;

    [Header("Child Components")]
    [SerializeField] private Image icon;
    [SerializeField] private TextMeshProUGUI nameTag;

    public void SetAction(Action _a)
    {
        action = _a;
        if (nameTag)
            nameTag.text = action.actionName;

        if(icon)
        {
            icon.sprite = action.icon;
            icon.color = action.colour;
        }
    }
}
