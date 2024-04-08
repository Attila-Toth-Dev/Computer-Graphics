using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerControls : MonoBehaviour
{
    [Header("Movement Settings")]
    [SerializeField] private float forwardSpeed = 300f;
    [SerializeField] private float rotationSpeed = 200f;
    [SerializeField] private float sprintMultiplier = 2f;

    [Header("Inputs")]
    [SerializeField] private InputActionReference movementReference;
    [SerializeField] private InputActionReference jumpReference;
    [SerializeField] private InputActionReference sprintReference;
    [SerializeField] private InputActionReference shootReference;

    private CharacterController controller;
    private Animator animator;

    private bool isSprinting;

    private Vector2 movement;

    private void Awake()
    {
        jumpReference.action.started += Jump;
        shootReference.action.started += Shoot;
    }

    private void Start()
    {
        controller = GetComponent<CharacterController>();
        animator = GetComponent<Animator>();
    }

    private void FixedUpdate()
    {
        Movement();
    }

    private void Animations()
    {
        animator.SetFloat("Forward", GetInputs().y);
        animator.SetFloat("Sense", GetInputs().y);

        animator.SetFloat("Turn", GetInputs().x);

        animator.SetBool("Sprint", isSprinting);
    }

    private Vector2 GetInputs()
    {
        movement = movementReference.action.ReadValue<Vector2>();
        return movement;
    }

    private void Movement()
    {
        Vector3 forwardMovement = transform.forward * GetInputs().y * forwardSpeed * Time.deltaTime;   

        // if sprint is held
        if(sprintReference.action.IsPressed())
        {
            isSprinting = true;
            controller.Move(forwardMovement * sprintMultiplier);
        }
        else
        {
            isSprinting = false;
            controller.Move(forwardMovement);
        }

        transform.Rotate(transform.up, GetInputs().x * rotationSpeed * Time.deltaTime);
        Animations();
    }

    private void Jump(InputAction.CallbackContext context)
    {
        
    }

    private void Shoot(InputAction.CallbackContext context)
    {
        animator.SetTrigger("Shoot");
    }

    private void OnEnable()
    {
        movementReference.action.Enable();

        jumpReference.action.Enable();
        sprintReference.action.Enable();
        shootReference.action.Enable();
    }

    private void OnDisable()
    {
        movementReference.action.Disable();

        jumpReference.action.Disable();
        sprintReference.action.Disable();
        shootReference.action.Disable();
    }
}
