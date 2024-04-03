using System;
using UnityEngine;
using static UnityEditor.Searcher.SearcherWindow.Alignment;

public class BasicPlayer : MonoBehaviour
{
    [SerializeField] private float forwardSpeed = 300f;
    [SerializeField] private float rotationSpeed = 200f;
    [SerializeField] private float pushPower = 2f;

    private CharacterController controller;
    private Animator animator;

    private float vertical;
    private float horizontal;

    private void Start()
    {
        controller = GetComponent<CharacterController>();
        animator = GetComponent<Animator>();
    }

    private void Update()
    {
        vertical = Input.GetAxis("Vertical");

        animator.SetFloat("Forward", Mathf.Abs(vertical));
        animator.SetFloat("Sense", Mathf.Sign(vertical));

        animator.SetFloat("Turn", Input.GetAxis("Horizontal"));
        //animator.SetFloat("Sense", Mathf.Sign(horizontal));
    }

    private void FixedUpdate()
    {
        Movement();
    }

    private void Movement()
    {
        controller.Move(transform.forward * vertical * Time.deltaTime);
        transform.Rotate(transform.up, horizontal * rotationSpeed * Time.deltaTime);
    }
}
