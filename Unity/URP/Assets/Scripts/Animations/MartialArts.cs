using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MartialArts : MonoBehaviour
{
    public GameObject punchFX;
    public GameObject kickFX;

    private Animator animator;

    private void Start()
    {
        animator = GetComponent<Animator>();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Alpha1))
        {
            animator.SetTrigger("Punch");
            if (Input.GetAxis("Horizontal") == 0 && Input.GetAxis("Vertical") == 0)
                animator.SetTrigger("PunchLower");
        }

        if (Input.GetKeyDown(KeyCode.Alpha2))
            animator.SetTrigger("Kick");
    }
}
