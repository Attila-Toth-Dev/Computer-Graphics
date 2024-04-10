using NaughtyAttributes;

using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerControls : MonoBehaviour
{
    [Header("Movement Settings")]
    [SerializeField] private Rigidbody rigidBody;
    
    [SerializeField] private float walkSpeed = 4f;
    [SerializeField] private float sprintSpeed = 10f;
    [SerializeField] private float rotationSpeed = 200f;
    [SerializeField] private float jumpForce = 100f;
    [SerializeField] private float jumpCooldown = 5f;
    [SerializeField, ReadOnly] private bool readyToJump;

    [Header("Debugging")]
    [SerializeField, ReadOnly] private Vector2 movement;
    [SerializeField, ReadOnly] private bool isGrounded;
    [SerializeField, ReadOnly] private bool isSprinting;
    [SerializeField, ReadOnly] private bool isCrouching;
    [SerializeField, ReadOnly] private bool isSlashing;
    [SerializeField, ReadOnly] private bool isBlocking;
    [SerializeField] private float maxGroundDistance = 0.5f;

    [Header("Input Actions")]
    [SerializeField] private InputActionReference movementActionRef;
    [SerializeField] private InputActionReference sprintActionRef;
    [SerializeField] private InputActionReference jumpActionRef;
    [SerializeField] private InputActionReference meleeActionRef;
    [SerializeField] private InputActionReference blockActionRef;
    [SerializeField] private InputActionReference crouchActionRef;

    private float moveSpeed;
    
    private Animator animator;
    
    private static readonly int forward = Animator.StringToHash("Forward");
    private static readonly int sense = Animator.StringToHash("Sense");
    private static readonly int turn = Animator.StringToHash("Turn");
    private static readonly int slash = Animator.StringToHash("Slash");

    private void Awake()
    {
        jumpActionRef.action.started += Jump;
        //crouchActionRef.action.started += Crouch;

        meleeActionRef.action.started += Melee;
        //blockActionRef.action.started += Block;
    }

    private void Start()
    {
        animator = GetComponent<Animator>();

        moveSpeed = walkSpeed;

        readyToJump = true;
    }

    private void Update()
    {
        GetInputs();
        GroundChecking();
        
        Movement();
    }

    private void GroundChecking()
    {
        isGrounded = Physics.Raycast(transform.position, Vector3.down, maxGroundDistance);
    }

    private void GetInputs()
    {
        movement = movementActionRef.action.ReadValue<Vector2>();
        
        isSprinting = sprintActionRef.action.IsInProgress();
    }

    private void Movement()
    {
        moveSpeed = isSprinting ? sprintSpeed : walkSpeed;

        rigidBody.AddForce(transform.forward * (Time.deltaTime * moveSpeed * movement.y), ForceMode.Acceleration);
        transform.Rotate(Vector3.up * (Time.deltaTime * rotationSpeed * movement.x));
        
        Animations();
    }

    private void Animations()
    {
        animator.SetBool("Falling", isGrounded);
        
        animator.SetFloat(forward, Mathf.Lerp(0, movement.y * moveSpeed, 10f));
        animator.SetFloat(sense, Mathf.Sign(movement.y));
        
        animator.SetFloat(turn, movement.x);
    }
    
    private void Jump(InputAction.CallbackContext context)
    {
        if(isGrounded)
        {
            readyToJump = false;

            rigidBody.velocity = new Vector3(rigidBody.velocity.x, 0f, rigidBody.velocity.z);
            rigidBody.AddForce(Vector3.up * jumpForce, ForceMode.Impulse);
            
            Invoke(nameof(ResetJump), jumpCooldown);
        }
    }

    private void Melee(InputAction.CallbackContext context)
    {
        animator.SetTrigger(slash);
    }
    
    private void ResetJump() => readyToJump = true;

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

    private void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Vector3 endPos = new (transform.position.x, transform.position.y - maxGroundDistance, transform.position.z);
        Gizmos.DrawLine(transform.position, endPos);
    }
}