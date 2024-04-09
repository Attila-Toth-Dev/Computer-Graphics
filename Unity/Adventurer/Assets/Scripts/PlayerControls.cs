using System;
using Unity.Collections;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerControls : MonoBehaviour
{
    [Header("Movement Settings")]
    [SerializeField] private float moveSpeed = 2.5f;
    [SerializeField] private float rotationSpeed = 200f;

    [Header("Inputs")]
    [SerializeField] private InputActionReference movementReference;
    [SerializeField] private InputActionReference jumpReference;
    [SerializeField] private InputActionReference sprintReference;
    [SerializeField] private InputActionReference meleeReference;

    private float sprintMultiplier;
    private bool isSprinting;

    private Animator animator;

    private Vector2 movement;

    private void Awake()
    {
        jumpReference.action.started += Jump;
        meleeReference.action.started += Shoot;
    }

    private void Start()
    {
        animator = GetComponent<Animator>();
    }

    private void FixedUpdate()
    {
        Movement();
    }

    private void Animations()
    {
        animator.SetFloat("Forward", Mathf.Abs(GetInputs().y));
        animator.SetFloat("Sense", Mathf.Sign(GetInputs().x));

        animator.SetFloat("Turn", GetInputs().x);
    }

    private Vector2 GetInputs()
    {
        isSprinting = sprintReference.action.IsPressed();

        movement = movementReference.action.ReadValue<Vector2>();
        return movement;
    }

    private void Movement()
    {
        sprintMultiplier = isSprinting ? 2f : 1f;

        Vector3 moveDir = new Vector3(GetInputs().x, 0, GetInputs().y);

        transform.Translate(moveDir * moveSpeed * sprintMultiplier * Time.deltaTime);

        transform.Rotate(transform.up, GetInputs().x * rotationSpeed * Time.deltaTime);
        Animations();

        Debug.Log(isSprinting ? "Sprinting" : "Not Sprinting");
        Debug.Log(sprintMultiplier);
    }

    private void Jump(InputAction.CallbackContext context)
    {
        
    }

    private void Shoot(InputAction.CallbackContext context)
    {
        animator.SetTrigger("Slash");
    }

    private void OnEnable()
    {
        movementReference.action.Enable();

        jumpReference.action.Enable();
        sprintReference.action.Enable();
        meleeReference.action.Enable();
    }

    private void OnDisable()
    {
        movementReference.action.Disable();

        jumpReference.action.Disable();
        sprintReference.action.Disable();
        meleeReference.action.Disable();
    }
}
