using NaughtyAttributes;
using System;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerControls : MonoBehaviour
{
    [Header("Movement Settings")]
    [SerializeField] private float walkSpeed = 2.5f;
    [SerializeField] private float sprintSpeed = 4f;
    [SerializeField] private float rotationSpeed = 10f;

    private float moveSpeed;

    [Header("Debugging")]
    [SerializeField, ReadOnly] private Vector2 movement;
    [SerializeField, ReadOnly] private bool _isWalking;
    [SerializeField, ReadOnly] private bool _isSprinting;
    [SerializeField, ReadOnly] private bool _isCrouching;
    [SerializeField, ReadOnly] private bool _isSlashing;
    [SerializeField, ReadOnly] private bool _isBlocking;

    [Header("Input Actions")]
    [SerializeField] private InputActionReference movementActionRef;
    [SerializeField] private InputActionReference sprintActionRef;
    [SerializeField] private InputActionReference jumpActionRef;
    [SerializeField] private InputActionReference meleeActionRef;
    [SerializeField] private InputActionReference blockActionRef;
    [SerializeField] private InputActionReference crouchActionRef;

    private Animator animator;
    private CharacterController controller;

    private void Awake()
    {
        //jumpActionRef.action.started += Jump;
        //crouchActionRef.action.started += Crouch;

        meleeActionRef.action.started += Melee;
        //blockActionRef.action.started += Block;
    }

    private void Start()
    {
        animator = GetComponent<Animator>();
        controller = GetComponent<CharacterController>();

        moveSpeed = walkSpeed;
    }

    private void Update()
    {
        GetInputs();

        Movement();
    }
    private void GetInputs()
    {
        movement = movementActionRef.action.ReadValue<Vector2>();
        _isSprinting = sprintActionRef.action.IsInProgress();
    }

    private void Movement()
    {
        controller.Move(transform.forward * movement.y * (moveSpeed = _isSprinting ? sprintSpeed : walkSpeed) * Time.deltaTime);
        transform.Rotate(transform.up, movement.x * rotationSpeed * Time.deltaTime);

        animator.SetFloat("Forward", Mathf.Lerp(-movement.y, movement.y, Time.deltaTime));
        animator.SetFloat("Sense", Mathf.Sign(movement.y));

        animator.SetFloat("Turn", movement.x);
    }

    private void Jump(InputAction.CallbackContext context)
    {
        
    }

    private void Melee(InputAction.CallbackContext context)
    {
        animator.SetTrigger("Slash");
    }

    private void OnEnable()
    {
        movementActionRef.action.Enable();
        crouchActionRef.action.Enable();
        sprintActionRef.action.Enable();
        jumpActionRef.action.Enable();

        meleeActionRef.action.Enable();
        blockActionRef.action.Enable();
    }

    private void OnDisable()
    {
        movementActionRef.action.Disable();
        crouchActionRef.action.Disable();
        sprintActionRef.action.Disable();
        jumpActionRef.action.Disable();

        meleeActionRef.action.Disable();
        blockActionRef.action.Disable();
    }
}
